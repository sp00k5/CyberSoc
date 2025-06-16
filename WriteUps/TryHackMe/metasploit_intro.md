# Metasploit: Introduction 
> [TryHackMe Link](https://tryhackme.com/room/metasploitintro)

The Metasploit Framework is a set of tools that allow information gathering, scanning, exploitation, exploit development, post-exploitation, it is also useful for vulnerability research and exploit development.
- **msfconsole**: The main command-line interface
- **Modules**: Small components within Metasploit built to perform a specific task, supporting modules such as exploits, scanners, payloads, etc
- **Tools**: Stand-alone tools that will help vulnerability research, vulnerability assessment, or penetration testing (e.g msfvenom, pattern_create and pattern_offset)

Useful definitions: 
- **Exploit**: A piece of code that uses a vulnerability present on the target system.
- **Vulnerability**: A design, coding, or logic flaw affecting the target system. The exploitation of a vulnerability can result in a attack
- **Payload**: Payloads are the code that will run on the target system. An exploit will take advantage of a vulnerability. However, if we want the exploit to have the result we want (gaining access to the target system, read confidential information, etc.), we need to use a payload. 


The console will be your main interface to interact with the different modules of the Metasploit Framework (`msfconsole`)

## Types of Modules

### Auxiliary
Any supporting module, such as scanners, crawlers and fuzzers, can be found here. 

```
auxiliary/
├── admin
├── analyze
├── bnat
├── client
├── cloud
├── crawler
├── docx
├── dos
├── example.py
├── example.rb
├── fileformat
├── fuzzers
├── gather
├── parser
├── pdf
├── scanner
├── server
├── sniffer
├── spoof
├── sqli
├── voip
└── vsploit
```
### Encoders
 Allow you to encode the exploit and payload in the hope that a signature-based antivirus solution may miss them. 

```
encoders/
├── cmd
├── generic
├── mipsbe
├── mipsle
├── php
├── ppc
├── ruby
├── sparc
├── x64
└── x86
```

### Evasion
Modules which will try to avoid antivirus detection, with more or less success.

```
evasion/
└── windows
    ├── applocker_evasion_install_util.rb
    ├── applocker_evasion_msbuild.rb
    ├── applocker_evasion_presentationhost.rb
    ├── applocker_evasion_regasm_regsvcs.rb
    ├── applocker_evasion_workflow_compiler.rb
    ├── process_herpaderping.rb
    ├── syscall_inject.rb
    ├── windows_defender_exe.rb
    └── windows_defender_js_hta.rb
```

### Exploits
Neatly organized by target system.

```
exploits/
├── aix
├── android
├── apple_ios
├── bsd
├── bsdi
├── dialup
├── example_linux_priv_esc.rb
├── example.py
├── example.rb
├── example_webapp.rb
├── firefox
├── freebsd
├── hpux
├── irix
├── linux
├── mainframe
├── multi
├── netware
├── openbsd
├── osx
├── qnx
├── solaris
├── unix
└── windows
```


### NOPs(No OPeration)
Do nothing, literally. They are represented in the Intel x86 CPU family with 0x90, following which the CPU will do nothing for one cycle. They are often used as a buffer to achieve consistent payload sizes.

```
nops/
├── aarch64
├── armle
├── cmd
├── mipsbe
├── php
├── ppc
├── sparc
├── tty
├── x64
└── x86
```

### Payloads
Payloads are codes that will run on the target system (e.g getting a shell, loading a malware or backdoor to the target system, running a command, or launching calc.exe as a proof of concept)
```
payloads/
├── adapters
├── singles
├── stagers
└── stages
```

- **Adapters**: An adapter wraps single payloads to convert them into different formats. For example, a normal single payload can be wrapped inside a Powershell adapter, which will make a single powershell command that will execute the payload.
- **Singles**: Self-contained payloads (add user, launch notepad.exe, etc.) that do not need to download an additional component to run.
- **Stagers**: Responsible for setting up a connection channel between Metasploit and the target system. Useful when working with staged payloads. “Staged payloads” will first upload a stager on the target system then download the rest of the payload (stage). This provides some advantages as the initial size of the payload will be relatively small compared to the full payload sent at once.
- **Stages**: Downloaded by the stager. This will allow you to use larger sized payloads.

Identify difference between single (*inline*) payloads and staged payloads: 
- generic/shell_reverse_tcp (an inline/single payload, as indicated by the “_” between “shell” and “reverse”)
- windows/x64/shell/reverse_tcp (staged payload, as indicated by the “/” between “shell” and “reverse”)

### Post
Post exploitation modules are useful on the final stage of the penetration testing process. 

```
post/
├── aix
├── android
├── apple_ios
├── bsd
├── firefox
├── hardware
├── linux
├── multi
├── networking
├── osx
├── solaris
└── windows
```

> **Note**: The format of a msfconsole module would be 
> `auxiliary/scanner/smb/smb_ms17_010` 
> module_type/category_of_module/protocol/attack

## MSFCONSOLE
Msfconsole is managed by context; meaning all configurations are bound to your current module (unless a global variable is set)

Context prompt: Once a module is in use and used the set command to chose it, the msfconsole will show the context. You can use context-specific commands (e.g. set RHOSTS 10.10.x.x) here
```
msf6 exploit(windows/smb/ms17_010_eternalblue) >
```

Meterpreter prompt: A Meterpreter agent was loaded to the target system and connected back to you. You can use Meterpreter specific commands here.
- Meterpreter is a Metasploit attack payload that provides an interactive shell from which an attacker can explore the target machine and execute code

Useful Commands: 
- `use` - select a module to use (can provide the number from a search result)
- `show options` - Provides informaiton on context set in which you will work (print options related to the exploit we have chosen earlier)
  - `show payloads` - List all payloads that can be used within a selected exploit 
- `back` - Leave current context
- `info` - Will display detailed information on the module such as its author, relevant sources, etc
- `search` - Will search the Metasploit Framework database for modules relevant to the given search parameter. You can conduct searches using CVE numbers, exploit names (eternalblue, heartbleed, etc.), or target system. (`search type:auxiliary telnet`)
- `set PARAMETER_NAME VALUE` - set parameter values which are required for modules
   -  `setg` - set values that will be used for all modules (`unsetg`)
- `unset` - clear any parameter value / clear all set parameters with the `unset all` 
- `exploit` - Launch the module 

> Note: Exploits are rated based on their reliability, marked as their [rank](https://docs.metasploit.com/docs/using-metasploit/intermediate/exploit-ranking.html)

It support bash commands whilst in the console.

## Parameters
Parameters you will often use are:
- **RHOSTS**: “Remote host”, the IP address of the target system. A single IP address or a network range can be set. This will support the CIDR (Classless Inter-Domain Routing) notation (/24, /16, etc.) or a network range (10.10.10.x – 10.10.10.y). You can also use a file where targets are listed, one target per line using file:/path/of/the/target_file.txt
- **RPORT**: “Remote port”, the port on the target system the vulnerable application is running on.
- **PAYLOAD**: The payload you will use with the exploit.
- **LHOST**: “Localhost”, the attacking machine (your AttackBox or Kali Linux) IP address.
- **LPORT**: “Local port”, the port you will use for the reverse shell to connect back to. This is a port on your attacking machine, and you can set it to any port not used by any other application.
- **SESSION**: Each connection established to the target system using Metasploit will have a session ID. You will use this with post-exploitation modules that will connect to the target system using an existing connection.

## Sessions 
Once a vulnerability has been successfully exploited, a session will be created. This is the communication channel established between the target system and Metasploit.
- `background` - background the session prompt and go back to the msfconsole prompt (CTRL + Z)
- `sessions` - Used from the msfconsole prompt or any context to see the existing sessions.
   - `-i <NUMBER>` - Interact with any available session









