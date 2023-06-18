# NOTES

Has anyone watched any hacking movie ever, they are cringe and don't really reflect the true side of networking. \
Reality is that it is slow and tedious to actually find what you need
and with so many factors.
- Propagation Delay
- Protocols
- Is it Client is it Server (who knows)


WI-FI is actually a brand and not an acronym, but with it being so integrated into people's lives it doesn't surprise me 

The two frequencies are 2.4 Ghz & 5 Ghz

## Trade off: 
### 2.4Ghz
-  Network Coverage (Lower frequency means greater distance can be travelled)
- Compatibility (older devices might not be able to support 802.11ac so need 802.11n)

### 5Ghz
- Network speed (higher frequency 5 GHz band makes up for its shorter range with much faster WiFi speeds)
- Co-Channel Interference (45 WiFi channels, where 24 of which are non-overlapping / but in 2.4 there are only 11 channels with 3 non-overlapping)

Wifi channel - smaller bands within WiFi frequency bands that are used by your wireless network to send and receive data

Overlapping - This is when everyone is trying to transmit within that region of overlap slowing down the network as the router deals with CSMA /TCA (Carrier-sense multiple access with collision avoidance)

# SLIDE 2 (WIFI Basics)

What is WAP?
No, its not Cardi B related? 

There are many different types of WAP that we won't get into today but here are a few:
- Root access point: an access point is connected directly to a wired LAN, providing a connection point for wireless users
- Repeater access point: mesh extender can be configured as a standalone repeater to extend the range of your infrastructure
- Bridges: Access points can be configured as root or non-root bridges to join multiple networks

Layer 2 is the datalink layer on the OSI model which defines the format of data on the network.

What defines a Layer 2 device: \
A frame is a protocol data unit, the smallest unit of bits on a Layer 2 network. Frames are transmitted to and received from devices on the same local area network (LAN). Unilke bits, frames have a defined structure and can be used for error detection, control plane activities and so forth. Not all frames carry user data. The network uses some frames to control the data link itself.

# SLIDE 3 (WIFI Basics pt2)

## Beacon Frames
Transmitted peridoitcally to announce the prescense of wireless networks.
Including whether it is in:
- Ad Hoc (does not rely on a pre-existing infrastructure, such as routers in wired networks or access points in wireless networks)
- Infrastu __WHAT WE ARE INTERESED__ IN (An infrastructure network is the network architecture for providing communication between wireless clients and wired network resources. The transition of data from the wireless to wired medium occurs via an AP)
- Capability Information  such as the channel and data rate

## SSID 
32 character human readable character string that identifies the network. This includes the timestamp which all associated stations will go ahead and synchronize to (done in hex)

## EAPOL
WPA2 (802.11a) where you are using a Pre-Shared Key (PSK) - your wireless password, a process called a 4-way handshake is used to establish identity and exchange the certificate identities between your client (phone, laptop etc.) and your wireless access poin

# SLIDE 4 (De-auth)
Unlike a normal DDOS attack where a user is sending over whelming amounts of packets, here in WIFI it is actually alot simpler as there is a frame to achieve this. Though a deauth can be annoying hackers can use tools like wifite to later build upon theser results by capturing your first copy of the initial handshake, they can try out various guesses at your passphrase and test whether they are correct. 

Wifite - is a tool to audit WEP or WPA encrypted wireless networks. It uses aircrack-ng, pyrit, reaver, tshark tools to perform the audit. This tool is customizable to be automated with only a few arguments and can be trusted to run without supervision

Management Frame Protection - a method of detecting these attacks and even protecting this type of attack if it is enabled and the client device supports it. This process was standardized with the IEEE 802.11w amendment released in 2009

# SLIDE 5 (ARP Spoofing)

Bettercap - BetterCAP is a powerful, flexible and portable tool created to perform various types of MITM attacks against a network, manipulate HTTP, HTTPS and TCP traffic in realtime, sniff for credentials, including ARP spoofing. 

__Mitigation__ \

__Static ARP Tables__ - Statically map all the MAC addresses in a network to their rightful IP addresses. This is highly effective in preventing ARP Poisoning attacks but adds a tremendous administrative burden. Any change to the network will require manual updates of the ARP tables across all host

__Packet filters__ - inspect packets as they are transmitted across 
a network. Packet filters are useful in ARP spoofing prevention
because they can filter out and blocking packets with 
conflicting source address information.

__Dynamic ARP Inspection__ - these features evaluate the validity of each ARP message and drop packets that appear suspicious or malicious. DAI can also typically be configured to limit the rate at which ARP messages can pass through the switch, effectively preventing DoS attacks

__Network Isolation__ - ARP messages don't travel beyond the local subnet

# SLIDE 6 (Beacon Flooding)

### Beacon Flooding is pretty bad though

MDK3 - injects random beacon frames onto the targeted channels toward
nearby wireless clients. This type of attack fills up the Wi-Fi network browserof the client with many spoofed wireless SSIDs, making it difficult to identifythe legitimate networks

### Probe Frames 

Active Scanning - the client station sends probe request frames with the SSID field set to null or a preferred SSID. The access points in the nearby range that hear this request will answer with the probe response frame. The probe response frame contains all the information that is present in the beacon frame. When a nonbeaconing AP is present in the vicinity, it will reply to the probe request, revealing its presence.

airodump-ng - scans for the devices using this method of active scanning 

Evil Twin - commonly used as an example when people discuss open wifi spots available such as in Starbucks. If someone had the SSID of your network then it would be able to send out probe requests and your device would connect. 

If this network has WEP or WPA then this most likely won't work since replicating a perfect copy (but not impossible)

Something quite interesting is that creating hidden networks with WIFI is actually quite difficult because of these probe frames as your device will be sending these requests out constantly 

There many other attacks and tools one could use to comprimise a network
- Session Hijacking
- Credential harvesting
- DNS spoof
...

## SLIDE 8 (WEP)

40 bit (10 Hex character) as "secret key" (set by user)
24 bit " Initialization Vector " (not under user control) 
(40+24=64)

Two modes
Shared Key Authentication -
Open System Authentication - Didn't require credentials
Can authenticate with the WAP but the WAP would begin communicating with client encrypting the data frames with the pre-shared WEP key


The decryption process is the reverse of the encryption process. 
First, the Initialization Vector (IV) is removed from the data packet and merged with the shared password.
This value is then used with KSA, and subsequently used to recreate the key stream. 
The stream and encrypted data packet are XORed together, which results in the plaintext output. 
The Integrity Check Value (ICV) is then removed from the plaintext and compared against a recalculated Integrity Check Value (ICV) and the packet is then either accepted or rejected according to the results of calculation.

__TKMAX__ -  Hackers cracked the WEP encryption protocol used to transmit data between price-checking devices, cash registers and computers at a store in Minnesota. The intruders then collected information submitted by employees logging on to the company's central database in Massachusetts, stealing usernames and passwords.

## SLIDE 9 (WPA)

The minimum password length of the WPA PSK key is eight ASCII characters.

authentication method in brief:
1. Each user is given a password to authenticate to the network; those who
have this password can access the network. The password, also called the Pre-shared Master Key (PMK), is the same for all the users.
2. By using this master key, a transient, or temporal, key is generated between the client and AP for every new session. Pairwise Transient Keys (PTK) are used to encrypt the data and the key is valid until the session ends.

## SLIDE 10 (HACK)
Unlike WPA/WPA2 PSK, a dictionary file is not required to crack the key; however, we can also crack the key using aircrack-ng along with a dictionary.

## SLIDE 10 (WPA2 / 3)

These all sound great and looks like we are on the brink of full securirty
I will finally be able to do my online banking in Star Bucks right?

Well no, as one door closes another opens and though these most of the time are great when it comes to networking you will never achieve perfection


