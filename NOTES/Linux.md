# Linux Fundamentals

## SLIDE (1)

Linux is a operating system made from a software collection that includes the Linux kernel and, often, a package management system. Linux features many tools and commands available to help traverse across the terminal.

**Flags** - are options used to modify the behavior of a command
(-v shows a version || --help gets man page)


Using CLI requires much less CPU and memory from the computer, allowing low-power computers to cheaply run complex programs without having to waste time drawing everything for a user.

**Kali Linux** - A open-source, Debian-based Linux distribution aimed at advanced Penetration Testing and Security"
(Feature Rich with security analysis, security auditing, and penetration testing tools)

## SLIDE (2)
`#Linux Bash Shell`

**Bash** - An application which is intended to conform to the shell standard specified as part of IEEE POSIX and it is available for most versions of Unix and Linux. Bash can be a a simple command/response system where users enter commands and bash returns the results after those commands are run. It can also be a programming platform and users are enabled to write programs that accept input and produce output using shell commands in shell scripts

Some basic commands you can use:
| Command | Use |
|---------|-----|
| cd | Change directory currently in terminal |
| pwd | Print current directory user is in |
| ls | List all contents in a directory |
| env | Allows you to display your current environment |
| echo | Used to display line of text/string that are passed as an argument |
| strings | Command looks for printable strings in a file |
| stat | Gives information about the file and filesystem (size of the file, access permissions, user ID, group ID, birth time, access time) |
| file | Determine the type of a file |
| find | Used to find files and directories and perform subsequent operations on them |
| sudo | Allows users to run commands with privileges that only root user have |

## SLIDE (3) 
`#Kernel and the Filesystem`

**Directory** - file system cataloging structure which contains references to other computer files.
The Linux operating system as a whole is broken down into these which define the directory structure and directory contents in Unix-like operating systems.

**Filesystem Hierarchy Standard (FHS)** - all Linux distributions are compliant with this universal standard for filesystem directory structure that is defined as a set of directories, each of which serve their own special function

| Directory | Use |
|---------|-----|
| **/bin** | » Contains binaries, that is, some of the applications and programs you can run |
| **/boot** | » Contains files required for starting your system |
| **/dev** | » Contains device files which are generated at boot time (new webcam or a USB) |
| **/etc** | » Stands for “Everything to configure,” as it contains most, if not all system-wide configuration files |
| **/home** | » Contains your users’ personal directories and split by users /home/tom /home/matt |
| **/lib** | » Contains libraries which are files containing code that your applications can use. They contain snippets of code that applications use it contains the all-important kernel modules. The kernel modules are drivers that make things like your video card, sound card, WiFi |
| **/media** | » Where external storage will be automatically mounted when you plug it in and try to access it |
| **/mnt** | » Where you can manually mount storage devices or partitions |
| **/opt** | » Where software you compile (that is, you build yourself from source code (Applications will end up in the /opt/bin directory and libraries in the /opt/lib directory) |
| **/proc** | » A virtual directory which contains information about your computer, such as information about your CPU and the kernel your Linux system is running |
| **/root** | » The home directory of the superuser of the system |
| **/run** | » System processes use it to store temporary data for their own nefarious reasons |
| **/sbin** | » Contains applications that only the superuser will need. You can use these applications with the sudo command |
| **/usr** | »  Contains a mix of directories which in turn contain applications, libraries, documentation, wallpapers, icons |
| **/sys** | »  A virtual directory wich contains information from devices connected to your computer |
| **/tmp** | » Contains temporary files, usually placed there by applications that you are running |
| **/var** | » Contains many things like logs in the /var/log subdirectories (Logs are files that register events that happen on the system). If something fails in the kernel, it will be logged in a file in /var/log |

### A diagram of the relation between all of these directories can be visualised below:

<img width="100%" src="https://www.linuxfoundation.org/hubfs/Imported_Blog_Media/standard-unix-filesystem-hierarchy-1500x826.png">

### » Kernel
The Linux Kernel is the core interface between a computer’s hardware and its processes working within the kernel space, its 4 jobs include:
- **Memory management** - Keep track of how much memory is used to store what, and where
- **Process management** - Determine which processes can use the central processing unit (CPU), when, and for how long
- **Device drivers**: Act as mediator/interpreter between the hardware and processes
- **System calls and security**: Receive requests for service from the processes

## SLIDE (4)
`#Environment`

**Shell** - A program provides access to an operating system's components, split between two types:
  - Command-line interface: A concise and efficient mode of interacting with the OS, without requiring the overhead of a graphic user interface (bash)
  - Graphical user interface: An interactive click and point based interactive approach to interacting with the system's programs

**Desktop Environments** - 

terminal / ssh /desktop environment / VM

## SLIDE (5)
`#Tools`

Useful stuff to aid

revshells 







