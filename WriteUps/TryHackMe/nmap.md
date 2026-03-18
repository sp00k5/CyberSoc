# Nmap Live Host Discovery
> [TryHackMe Link](https://tryhackme.com/room/nmap01)
> [nmap Basics](https://tryhackme.com/room/nmap02)

Network Mapper is a open-source software created by Gordon Lyon (Fyodor). A industry-standard tool for mapping networks, identifying live hosts, and discovering running services. A Nmap scan usually goes through the following steps
- `Enumerate Targets` -> `Discover Live hosts` -> `Reverse-DNS lookup` -> `Scan ports`-> `Detect versions` -> `Detect OS` -> `Traceroute` -> `Scripts` -> `Write Output`

**subnetwork** - One or more network segments connected and configured to use the same router (logical connection). Has its own IP address range and is connected to a more extensive network via a router
**network segment** - A group of computers connected using a shared medium (ethernet switch, WiFi AP) (physical connection)

> **Note**: Nmap will attempt to connect to the 1000 most common ports by default but you can use `-F` to enable fast mode and decrease the number of scanned ports from 1000 to 100 most common ports.

`-r` - To scan the ports in consecutive order instead of random order.

## [Scanning Types](https://nmap.org/book/man-port-scanning-techniques.html)
Nmap can scan a 
- list: `MACHINE_IP` `scanme.nmap.org` `example.com` will scan 3 IP addresses
- range: `10.11.12.15-20` will scan 6 IP addresses: 10.11.12.15, 10.11.12.16
- subnet: `MACHINE_IP/30` will scan 4 IP addresses
- file: `nmap -iL list_of_hosts.txt`

> **Note**: `nmap -sL TARGETS` can be used to check the list of hosts which will be scanned without starting the scanning

Nmap will attempt reverse-DNS resolution on all targets to obtain their names (to prevent this add `-n`)


To solve **10.10.12.13/29** on paper, follow this step-by-step method.

The **/29** is the CIDR prefix length.

* IPv4 addresses have **32 total bits**
* **/29** means **29 bits are network bits**
* That leaves **3 bits for hosts** (32 − 29 = 3)

Now calculate total addresses:
- 2^3 = 8  (total IP addresses)


So every /29 subnet contains:

* 8 total addresses
* 6 usable host addresses
* 1 network address
* 1 broadcast address

---

# Step 2️⃣ Determine the Subnet Mask

/29 in binary:

```
11111111.11111111.11111111.11111000
```

Convert last octet to decimal:

```
11111000 = 248
```

So the subnet mask is:

```
255.255.255.248
```

---

# Step 3️⃣ Find the Block Size

Block size = 256 − 248 = **8**

That means subnets increase by **8** in the last octet.

The subnets in 10.10.12.x would be:

```
10.10.12.0
10.10.12.8
10.10.12.16
10.10.12.24
10.10.12.32
...
```

---

# Step 4️⃣ Find Which Subnet 10.10.12.13 Belongs To

We look for which multiple of 8 contains 13.

Multiples of 8:

* 8
* 16

Since:

```
8 ≤ 13 < 16
```

The subnet is:

```
10.10.12.8/29
```

---

# Step 5️⃣ Determine the Address Range

If subnet starts at **10.10.12.8** and block size is 8:

Network Address:

```
10.10.12.8
```

Add 7 (because 8 addresses total):

Broadcast Address:

```
10.10.12.15
```

So the full range is:

```
10.10.12.8 – 10.10.12.15
```

---

# Step 6️⃣ Determine Usable Host Range

* First usable: **10.10.12.9**
* Last usable: **10.10.12.14**

Total usable hosts: **6**

---

Given: **10.10.12.13/29**

* Subnet: **10.10.12.8/29**
* Subnet mask: **255.255.255.248**
* Network address: **10.10.12.8**
* Broadcast address: **10.10.12.15**
* Usable range: **10.10.12.9 – 10.10.12.14**
* Total usable hosts: **6**


We can leverage the following protocols to discover the live hosts.
- ARP from Link Layer
- [ICMP](https://www.iana.org/assignments/icmp-parameters/icmp-parameters.xhtml) from the Network Layer
- TCP from the Transport Layer
- UDP from the Transport Layer

When no host discovery options are provided, Nmap follows the following approaches to discover live hosts:

1. When a privileged user (root, sudoers) tries to scan targets on a local network (Ethernet), Nmap uses ARP requests.
2. When a privileged user tries to scan targets outside the local network, Nmap uses ICMP echo requests, TCP ACK (Acknowledge) to port 80, TCP SYN (Synchronize) to port 443, and ICMP timestamp request.
3. When an unprivileged user tries to scan targets outside the local network, Nmap resorts to a TCP 3-way handshake by sending SYN packets to ports 80 and 443.

Nmap, by default, uses a ping scan to find live hosts, then proceeds to scan live hosts only. If you want to use Nmap to discover online hosts without port-scanning the live systems, you can issue `nmap -sn TARGETS`

## Nmap with ARP

Address Resolution Protocol (ARP) queries to discover live hosts. An ARP query aims to obtain the hardware address (MAC address) so that communication at the link layer and ARP packets are bound to their subnet.
- The MAC address is required for the link-layer header; it contains the source and destination MAC addresses, among other fields
- `nmap -PR -sn TARGETS` = perform an ARP scan without port-scanning

### arp-scan

[arp-scan](https://github.com/royhills/arp-scan/wiki/arp-scan-User-Guide) is a command-line tool for IPv4 host discovery and fingerprinting
- `sudo arp-scan -I eth0 -l` - Send ARP queries for all valid IP addresses on the eth0 interface.

## Nmap with ICMP
Ping every IP address on a target network and see who would respond to our ping (ICMP Type 8/Echo) requests with a ping reply (ICMP Type 0).
- Many firewalls block this and MS Windows are configured with a host firewall that blocks ICMP echo requests by default
- `nmap -PE -sn TARGETS` = perform an ICMP echo request to discover live hosts
- `nmap -PP -sn MACHINE_IP` = perform ICMP Timestamp using a timestamp request (ICMP Type 13) and checks whether it will get a Timestamp reply (ICMP Type 14)
- `nmap -PM -sn MACHINE_IP/24` = perform ICMP Address Mask using address mask queries (ICMP Type 17) and checks whether it gets an address mask reply (ICMP Type 18)

## Nmap with TCP

A TCP port or UDP port is used to identify a network service running on that host  and port is usually linked to a service using that specific port number:
-  HTTP server would bind to TCP port 80
-  HTTP server supports SSL/TLS, it would listen on TCP port 443

Can classify ports in namp by the following states:
- **Open port** indicates that there is some service listening on that port.
- **Closed port** indicates that there is no service listening on that port. Although the port is accessible. By accessible, we mean that it is reachable and is not blocked by a firewall or other security appliances/programs.
- **Filtered** indicates Nmap cannot determine if the port is open or closed because the port is not accessible (usually due to a firewall preventing Nmap from reaching that port or receiving)
- **Unfiltered** indicates that Nmap cannot determine if the port is open or closed, although the port is accessible (encountered with an ACK scan)
- **Open|Filtered** indicates that Nmap cannot determine whether the port is open or filtered
- **Closed|Filtered** indicated that Nmap cannot decide whether a port is closed or filtered.

The TCP header is the first 24 bytes of a TCP segment and is defined in [RFC 793](https://datatracker.ietf.org/doc/html/rfc793.html). Setting a flag bit means setting its value to 1 and NMAP can do this for 6 flags:

- **URG**: Urgent flag indicates that the urgent pointer filed is significant. The urgent pointer indicates that the incoming data is urgent, and that a TCP segment with the URG flag set is processed immediately without consideration of having to wait on previously sent TCP segments.
- **ACK**: Acknowledgement flag indicates that the acknowledgement number is significant. It is used to acknowledge the receipt of a TCP segment.
- **PSH**: Push flag asking TCP to pass the data to the application promptly.
- **RST**: Reset flag is used to reset the connection. Another device, such as a firewall, might send it to tear a TCP connection. This flag is also used when data is sent to a host and there is no service on the receiving end to answer.
- **SYN**: Synchronize flag is used to initiate a TCP 3-way handshake and synchronize sequence numbers with the other host. The sequence number should be set randomly during TCP connection establishment.
- **FIN**: The sender has no more data to send.

### TCP Connect Scan
`nmap -sT TARGET`- The connection is torn as soon as its state is confirmed by sending a RST/ACK (default scan as a non root user)

Any open TCP port will require Nmap to complete the TCP 3-way handshake before closing the connection.

### TCP SYN Ping
Send a packet with the SYN (Synchronize) flag set to a TCP port, 80 by default, and wait for a response (`-PSPORT` flag)
- open port should reply with a SYN/ACK (Acknowledge)
- a closed port would result in an RST (Reset).

Privileged users (root and sudoers) can send TCP SYN packets and don’t need to complete the TCP 3-way handshake (RST is sent back after the SYN, ACK)
- `nmap -sS TARGET`

### TCP ACK Ping
Sends a packet with an ACK flag set (privileged user required accomplish this)
- `nmap -PA -sn TARGET` = sends a ACK TCP packet on port 80 and should receive a TCP packet with the RST flag set

### UDP Ping
As UDP is connectionless and does not need a handshake it can't be guaranteed that a service listening on a UDP port would respond to our packets.

If we send a UDP packet to a closed UDP port, we expect to get an ICMP port unreachable packet  (type 3, destination unreachable, and code 3, port unreachable); this indicates that the target system is up and available.
- `nmap -PU -sn TARGET` = send a UDP packet to a port and expect a ICMP Type 3, Code 3 reply
- `nmap -sU TARGET` is the me thing

> Note: A tool like [Masscan](https://github.com/robertdavidgraham/masscan) can perform similar TCP host discoveries with a much more aggressive approach `masscan MACHINE_IP/24 ‐‐top-ports 100` 

Nmap’s default behaviour is to use reverse-DNS online hosts. Nmap will look up online hosts; however, you can use the option `-R` to query the DNS server even for offline hosts.
- If you want to use a specific DNS server, you can add the `--dns-servers DNS_SERVER` option

### Null Scan
All 6 bits are set to 0 in TCP packet.  A TCP packet with no flags set will not trigger any response when it reaches an open port meaning the port is open or blocked by firewall. Expect the target server to respond with an RST packet if the port is closed.
- can be chosen with `-sN` flag

### FIN Scan
Sends a TCP packet with the FIN flag set. Similarly to Null Scan no response will be sent if the port is open and an RST is sent if closed
- can be chosen with `-sF` flag. 

### Xmas Scan
Sets the FIN, PSH, and URG flags simultaneously. If an RST packet is received, it means that the port is closed. Otherwise, it will be reported as open|filtered
- can be chosen with `-sX` flag. 

> **Note**: A Null, FIN and Xmas scan can be useful when dealing with a **stateless firewall** as it will be searching for a SYN flag to detect connection attempts. A stateful firewall will block these payloads

### TCP Maimon Scan
The FIN and ACK bits are set and the target should send an RST packet as a response. However, certain BSD-derived systems drop the packet if it is an open port exposing the open ports
- can be chosen with `-sM` flag. 
- Most target systems respond with an RST packet regardless of whether the TCP port is open

### TCP ACK Scan
Will send a packet with the ACK flag set and the target would respond to the ACK with RST regardless of the state of the port
- can be chosen with `-sA` flag. 
- This kind of scan would be helpful if there is a firewall in front of the target as you will learn which ports were not blocked by the firewall

### Window Scan
Identical to the ACK Scan by sending a ACK packet however examines the TCP Window field from the RST packet returned. On some systems, open ports use a positive window size (even for RST packets) while closed ones have a zero window
- can be chosen with `-sW` flag. 
- Similary if there was a firewall and we received that ports are closed we can identify the firewall not blocking those ports 

### Custom Scan
Can customise combinations of scans with which flags to set using ` --scanflags` 
- e.g `--scanflags RSTSYNFIN`

## Options
Can specify which ports to scan using 
- port list: `-p22,80,443` will scan ports 22, 80 and 443
- port range: `-p1-1023` will scan all ports between 1 and 1023 inclusive
- all ports: `-p-`

### Scan Timing 
Using `-T<0-5>`
- paranoid (0) scans one port at a time and waits 5 minutes between sending each probe
- sneaky (1)
- polite (2)
- normal (3) nmap normally uses this
- aggressive (4) Used during CTFs and when learning to scan on practice target
- insane (5) most aggressive in terms of speed; however, this can affect the accuracy of the scan due to packet loss

> **Note**: avoid IDS alerts with T0 and T1

Control the packet rate using `--min-rate <number>` and `--max-rate <number>`.
- Example: `--max-rate 10` or `--max-rate=10` ensures that your scanner is not sending more than ten packets per second


Control probing parallelization using `--min-parallelism <numprobes>` and `--max-parallelism <numprobes>`. 
- Nmap probes the targets to discover which hosts are live and which ports are open
- Probing parallelization specifies the number of such probes that can be run in parallel.
- Example: `--min-parallelism=512` pushes Nmap to maintain at least 512 probes in parallel; these 512 probes are related to host discovery and open ports.

### Details 
- `--reason` flag gives the explicit reason why Nmap concluded that the system is up or a particular port is open
- `-v` and `-vv` for verbose outputs
- `-d` and `-dd` for debugging outputs

### Spoofing
You can scan a target from a spoofed IP and MAC address to gurantee a capture of response. The target machine will respond to the incoming packets sending the replies to the real destination IP address that was spoofed
- `nmap -e NET_INTERFACE -Pn -S SPOOFED_IP MACHINE_IP`
- the -e specifies the network interface and -Pn and not to expect a ping reply
- Must be able to monitor the network otherwise you can't see reponses 

If on the same subnet and same interface as the target you can spoof your mac address with `--spoof-mac SPOOFED_MAC`
- attacker might resort to using decoys to make it more challenging to be pinpointed making it appear like scan came from many sources
- Launch a decoy scan by specifying a specific or random IP address after `-D` flag
  - `ME` used to represent attacker
  - `RND` used to generate random IP assigned
  - e.g `nmap -D 10.10.0.1,10.10.0.2,RND,RND,ME 10.129.140.61`

### Fragmentation
The IP data will be divided into 8 bytes or less. This means the data block will be broken into chunks across multiple packets and tracked using the ID annd fragment offset values in the header
- can be chosen with `-f` flag or `-ff` for 16 bytes
- `--mtu` can set a default value but should be a multiple of 8
- -`-data-length NUM` where num specifies the number of bytes you want to append to your packets (if you wanted to pad them)

### Idle/Zombie Scan
Requires an idle system connected to the network that you can communicate with and nmap will make each probe appear as if coming from the idle host then it will check for indicators whether the idle host received any response to the spoofed probe. 
- `nmap -sI ZOMBIE_IP TARGET_IP`

The idle scan requires the following three steps to discover whether a port is open:
1. Trigger the idle host to respond so that you can record the current IP ID on the idle host.
2. Send a SYN packet to a TCP port on the target. The packet should be spoofed to appear as if it was coming from the idle host IP address.
3. Trigger the idle machine again to respond so that you can compare the new IP ID with the one received earlier. 
4.  The attacker sends another SYN/ACK to the idle host. The idle host responds with an RST packet, incrementing the IP ID by one again. The attacker needs to compare the IP ID of the RST packet received in the first step with the IP ID of the RST packet received in this third step. If the difference is 1, it means the port on the target machine was closed or filtered. However, if the difference is 2, it means that the port on the target was open