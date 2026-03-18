# Active Reconnaissance
> [TryHackMe Link](https://tryhackme.com/room/activerecon)

Requires you to make some kind of contact with your target, usually as part of social engineering. Alternatively, it can be a direct connection to the target system.

> Note: It is essential to remember not to engage in active reconnaissance work before getting signed legal authorization from the client.

## Web Browser

It is readily available on all systems. There are several ways where you can use a web browser to gather information about a target. On the transport level, the browser connects to:
- TCP port 80 by default when the website is accessed over HTTP
- TCP port 443 by default when the website is accessed over HTTPS

###  Developer Tools 
Inspect what your browser has received and exchanged with the remote server
- view and even modify the JavaScript (JS) files
- inspect the cookies set on your system 
- discover the folder structure of the site content

There are many extensions which assist with active recon: 
- **FoxyProxy**: Lets you quickly change the proxy server you are using to access the target website (Burp Suite intercept)
- **User-Agent Switcher and Manager**: Provides the ability to pretend to be accessing the webpage from a different operating system or different web browser.
- **Wappalyzer**: Provides insights about the technologies used on the visited websites. Such extension is handy, primarily when you collect all this information while browsing the website like any other user.

## Ping
Primary purpose of ping is to check whether you can reach the remote system and that the remote system can reach you back (checking whether the remote system is online).
- sends an **ICMP Echo packet** (ICMP echo/type 8) to a remote system, if the remote system is online, and the ping packet was correctly routed and not blocked by any firewall, the remote system should send back an **ICMP Echo Reply** (ICMP echo reply/type 0).

Sending 10 ICMP packets:
- Linux = `ping -c 10 MACHINE_IP`
- MS Windows = `ping -n 10 MACHINE_IP`

Why we didn’t get a ping reply:
- Destination computer is not responsive; possibly still booting up or turned off, or the OS has crashed.
- It is unplugged from the network, or there is a faulty network device across the path.
- A firewall is configured to block such packets. The firewall might be a piece of software running on the system itself or a separate network appliance. Note that MS Windows firewall blocks ping by default.
- Your system is unplugged from the network.

## Traceroute

Traces the route taken by the packets from your system to another host. Used to find the IP addresses of the routers or hops that a packet traverses as it goes from your system to a target host (revealing the number of routers between two systems).

- Linux = `traceroute MACHINE_IP`
- MS Windows = `tracert MACHINE_IP`

Rely on ICMP to have routers reveal their IP addresses. 
- Using a small Time To Live (TTL) in the IP header field it indicates the maximum number of routers/hops that a packet can pass through before being dropped.
- When a router receives a packet, it decrements the TTL by one before passing it to the next router.
- If the TTL reaches 0, it will be dropped, and an ICMP Time-to-Live exceeded would be sent to the original sender (some routers are configured not to send such ICMP messages when discarding a packet)
- On Linux, traceroute will start by sending UDP datagrams within IP packets of TTL being 1.

### Example
System sends three packets with TTL set to 1

`2  100.66.8.86 (100.66.8.86)  43.231 ms 100.65.21.64 (100.65.21.64)  18.886 ms 100.65.22.160 (100.65.22.160)  14.556 ms`
- Three different routers handled each request

`12 99.83.69.207 (99.83.69.207) 17.603 ms 15.827 ms 17.351 ms` 
- 12th router with the IP handled all three requests
- Shows the time in milliseconds for each reply to reach our system

`3 * 100.66.16.176 (100.66.16.176) 8.006 ms *`
- Indicate that our system didn’t receive two expected ICMP time exceeded in-transit messages.

> **Note**: The route taken by the packets might change as many routers use dynamic routing protocols that adapt to network changes.

## Telnet
Developed in 1969 to communicate with a remote system via a command-line interface. Uses the TELNET protocol for remote administration and the default port used is 23. Telnet sends all the data, including usernames and passwords, in cleartext making it unsecure.
- telnet client relies on the TCP protocol, you can use Telnet to connect to any service and grab its banner
- Connect to any service running on TCP and exchange messages unless it uses encryption

`telnet MACHINE_IP PORT`

### HTTP Example
Connect to the server at port 80, and then we communicate using the HTTP protocol
- `telnet MACHINE_IP 80`
- `GET / HTTP/1.1.`
- `host: example` (to get a valid response, instead of an error, you need to input some value for the host)

## Netcat (nc)
Supports both TCP and UDP protocols. It can function as a client that connects to a listening port; alternatively, it can act as a server that listens on a port of your choice. 

`nc MACHINE_IP PORT` 
- This can also run the same process as the telnet command for banner grabbing

### Server Setup
To open a port and listen on it, you can issue `nc -lp 1234` or `nc -vnlp 1234` 

| Flag | Description |
| ---- | ----------- |
|  -l  |  Listen Mode           |  
|  -p  |  Specify Port           | 
|  -n  |  Numeric Only, no resolution of hostnames via DNS           | 
|  -v  |  Verbose output            | 
|  -vv  | Very Verbose            | 
|  -k  |  Keep listening after client disconnects           | 

- `-p` should appear just before the port number you want to listen on
- `-n` will avoid DNS lookups and warnings
- port numbers less than 1024 require root privileges to listen on

### Client Setup 
`nc MACHINE_IP PORT` after you successfully establish a connection to the server, whatever you type on the client-side will be echoed on the server-side