# Passive Reconnaissance
> [TryHackMe Link](https://tryhackme.com/room/passiverecon)

Reconnaissance can be defined as a preliminary survey to gather information about a target. It is the first step in The [Unified Kill Chain](https://www.unifiedkillchain.com/) to gain an initial foothold on a system. 

In passive reconnaissance, you rely on publicly available knowledge. It is the knowledge that you can access from publicly available resources without directly engaging with the target. Activities may include: 
- Querying DNS records
- Checking related job ads
- Reading news articles about the target

## Whois
WHOIS is a request and response protocol that follows the [RFC 3912](https://www.ietf.org/rfc/rfc3912.txt) specification.  A WHOIS server listens on TCP port 43 for incoming requests. The domain registrar is responsible for maintaining the WHOIS records for the domain names it is leasing. The WHOIS server replies with various information related to the domain requested including: 
- **Registrar**: Which registrar was the domain name registered?
- **Contact info of registrant**: Name, organization, address, phone, among other things. (unless made hidden via a privacy service)
- **Creation, update, and expiration dates**: When was the domain name first registered? When was it last updated? And when does it need to be renewed?
- **Name Server**: Which server to ask to resolve the domain name?

`whois DOMAIN_NAME` to query WHOIS servers
- Here you would be re-directed to who is maintaining the WHOIS record for the domain


## nslookup

Stands for Name Server Look Up and can be used to find the IP or domain associated with an IP using the command below:
- `nslookup OPTIONS DOMAIN_NAME SERVER` to query DNS servers

It also offers parameters: 
- **OPTIONS** contains the query type. For instance, you can use A for IPv4 addresses and AAAA for IPv6 addresses (CNAME,MX,SOA,TXT)
- **DOMAIN_NAME** is the domain name you are looking up.
- **SERVER** is the DNS server that you want to query. You can choose any local or public DNS server to query.
  - Cloudflare offers `1.1.1.1` and `1.0.0.1`
  - Google offers `8.8.8.8` and `8.8.4.4`
  - Quad9 offers `9.9.9.9` and `149.112.112.112`

Example: `nslookup -type=MX tryhackme.com 1.1.1.1` would result in 
```
Non-authoritative answer:
tryhackme.com	mail exchanger = 5 alt1.aspmx.l.google.com.
tryhackme.com	mail exchanger = 1 aspmx.l.google.com.
tryhackme.com	mail exchanger = 10 alt4.aspmx.l.google.com.
```
Since MX is looking up the Mail Exchange servers, when a mail server tries to deliver email @tryhackme.com, it will try to connect to the `aspmx.l.google.com`, which has order 1. If it is busy or unavailable, the mail server will attempt to connect to the next in order mail exchange servers.

## dig

Domain Information Groper provides more advanced DNS queries and additional functionality. 

- `dig @SERVER DOMAIN_NAME TYPE` to query DNS servers

Example: `dig @1.1.1.1 tryhackme.com MX`

## [DNSDumpster](https://dnsdumpster.com/) 
The domain you are inspecting might include a different subdomain that can reveal much information about the target. DNSDumpster will return the collected DNS information in easy-to-read tables and a graph. DNSDumpster will also provide any collected information about listening servers.
- DNSDumpster will also represent the collected information graphically.

## [Shodan.io](https://www.shodan.io/)
Shodan.io tries to connect to every device reachable online to build a search engine of connected “things” in contrast with a search engine for web pages. 

Once it gets a response, it collects all the information related to the service and saves it in the database to make it searchable. With this you can learn about connected and exposed devices belonging to your organization including: 

- IP address
- hosting company
- geographic location
- server type and version
