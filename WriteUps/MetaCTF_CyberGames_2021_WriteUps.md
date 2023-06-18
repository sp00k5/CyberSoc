# MetaCTF CyberGames 2021
[Official WriteUps](https://ctftime.org/event/1476)

## Infilltrate 1
[Useful Help](https://ubuntu.com/server/docs/network-configuration)
1. ip addr add 192.168.0.2/24 dev eth0
2. ip addr del 192.168.0.3/24 dev eth0
3. ip link set dev eth0 up
4. ip address show dev eth0
5. ping 192.168.0.1
6. tcpdump -i eth0 udp port 8000 -vv -X 

## Infilltrate 3 
1. telnet 192.168.0.1 (user: root pass: admin)
2. ip addr add 172.16.0.2/24 dev enp1s0
3. ip link set dev enp1s0 up
4. nc -lu 8000
