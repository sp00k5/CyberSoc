# MetaCTF CyberGames 2021
Official WriteUps = https://ctftime.org/event/1476

## Infilltrate 1
https://ubuntu.com/server/docs/network-configuration
ip addr add 192.168.0.2/24 dev eth0
ip addr del 192.168.0.3/24 dev eth0
ip link set dev eth0 up
ip address show dev eth0
ping 192.168.0.1
tcpdump -i eth0 udp port 8000 -vv -X 

## Infilltrate 3 
telnet 192.168.0.1 (user: root pass: admin)
ip addr add 172.16.0.2/24 dev enp1s0
ip link set dev enp1s0 up
nc -lu 8000
