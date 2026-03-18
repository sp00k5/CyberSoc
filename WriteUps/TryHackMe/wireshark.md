# Wireshark: The Basics
> [TryHackMe Link](https://tryhackme.com/room/wiresharkthebasics)

Wireshark is an open-source, cross-platform network packet analyser tool capable of sniffing and investigating live traffic and inspecting packet captures (PCAP). 

It can be used to: 
- Detecting and troubleshooting network problems, such as network load failure points and congestion.
- Detecting security anomalies, such as rogue hosts, abnormal port usage, and suspicious traffic.
- Investigating and learning protocol details, such as response codes and payload data.

## WireShark Views 
When wireshark is open the first page visible shows the following:

| Tool | Description |
| -----| ----------- |
| **Toolbar** | contains multiple menus and shortcuts for packet sniffing and processing, including filtering, sorting, summarising, exporting and merging |
| **Display Filter Bar** | The main query and filtering section. |
| **Recent Files** | List of the recently investigated files. You can recall listed files with a double-click. |
| **Capture Filter and Inteface** | Capture filters and available sniffing points (network interfaces). The software connection (e.g., lo, eth0 and ens33) enables networking hardware.|  
| **Status Bar** | Tool status, profile and numeric packet information |

### PCAP View
When a pcap file is opened there are three different panes for viewing the packet information 

| Pane | Description |
| -----| ----------- |
| **Packet List Pane** | Summary of each packet (source and destination addresses, protocol, and packet info). You can click on the list to choose a packet for further investigation. Once you select a packet, the details will appear in the other panels. |
| **Packet Details Panel** | Detailed protocol breakdown of the selected packet. |
| **Packet Bytes Pane** | Hex and decoded ASCII representation of the selected packet. It highlights the packet field depending on the clicked section in the details pane. |

### Colour Packets
Wireshark also colour packets in order of different conditions and the protocol to spot anomalies and protocols in captures quickly
- You can create custom colour rules to spot events of interest by using display filters
- 
Wireshark has two types of packet colouring methods: 
- **Temporary rules** - Are only available during a program session
- **Permanent rules** - Are saved under the preference file (profile) and available for the next program session.

> Note: You can list the colour rules by visiting `View -> Coloring Rules` 

### Merge PCAPs
Wireshark can combine two pcap files into one single file. You can use the `File --> Merge` menu path to merge a pcap with the processed one.

### File Details
If you need to know the file details including (File hash, capture time, capture file comments, interface and statistics) to identify the file, classify and prioritise it. You can view the details by following `Statistics --> Capture File Properties` or by clicking the `pcap` icon located on the left bottom.

## Packet Dissection
Also known as protocol dissection, investigates packet details by decoding available protocols and fields. Wireshark supports a variety of protocols for dissection and provides the ability for dissection scripts. You can find more details on dissection [here](https://github.com/boundary/wireshark/blob/master/doc/README.dissector).

Packets consist of 5 to 7 layers based on the OSI model. You can see these levels broken down in the Packet Details Pane

### Example 
In a HTTP packet you can see the seven distinct layers of the packet

- **Frame/Packet** (Layer 1): What frame/packet you are looking at and details specific to the Physical layer of the OSI model.
- **Source [MAC]** (Layer 2): The source and destination MAC Addresses; from the Data Link layer of the OSI model.
- **Source [IP]** (Layer 3): The source and destination IPv4 Addresses; from the Network layer of the OSI model.
- **Protocol** (Layer 4): Details of the protocol used (UDP/TCP) and source and destination ports; from the Transport layer of the OSI model.
- **Protocol Errors**: Continuation of the 4th layer shows specific segments from TCP that needed to be reassembled.
- **Application Protocol** (Layer 5): Details specific to the protocol used, such as HTTP, FTP, and SMB. From the Application layer of the OSI model.
- **Application Data**: This extension of the 5th layer can show the application-specific data.

- Q1: Markup Language of the HTTP packet is described as part of the application data
- Q2: Arrival date of the packet is found within the frame layer 1
- Q3: TTL value crucial field in the IP header at the OSI Model's Layer 3 (Network Layer), designed to prevent packets from looping infinitely in a network by having routers decrement the value by one at each hop
- Q4: TCP is the protocol so its size will be in the protocol layer 4 

## Packet Navigation

#### Go Packet
Allows you to jump to specific packet based on its index number 

#### Find Packet 
Used to make a search inside the packets for a particular event of interest.
- This functionality accepts four types of inputs (Display filter, Hex, String and Regex). Searches are case insensitive, but you can set the case sensitivity in your search by clicking the radio button.
- Can conduct searches in the three panes (packet list, packet details, and packet bytes)
- Time Display Format lists the packets as they are captured and by default, Wireshark shows the time in "Seconds Since Beginning of Capture" but this can be changed

#### Mark Packets
Marked packets will be shown in black regardless of the original colour and will be lost after closing the capture file.

#### Packet Comments
Can add comments for particular packets and the comments can stay within the capture file until the operator removes them.

#### Export Packets
Separate a selected group of specific packages from the file and dig deeper to resolve an incident.

#### Extract Objects
Extract files transferred through the wire. Exporting objects are available only for selected protocol's streams (DICOM, HTTP, IMF, SMB and TFTP).

#### Info
Wireshark detects and suggests specific states of protocols to help analysts easily spot possible anomalies and problems (chance of having false positives/negatives). Expert info can provide a group of categories in three different severities:

| Severity | Colour | Info |
| -----| ----------- | ------ |
| Chat | Blue | Information on usual workflow. |
| Note | Cyan | Notable events like application error codes. |
| Warn | Yellow |Warnings like unusual error codes or problem statements. |
| Error | Red | Problems like malformed packets. |

## Packet Filtering

The filters are only the option to investigate the event of interest, there are two different ways to filter traffic and remove the noise from the capture file.
- Queries 
- Right-click menu

Wireshark has two types of filtering approaches
- Capture: filters are used for "capturing" only the packets valid for the used filter. 
- Display: filters are used for "viewing" the packets valid for the used filter.

### Filter Types
- **Apply as Filter**: The most basic way of filtering traffic. Right click on the field you want to filter and use "Analyse --> Apply as Filter" menu to filter the specific value. Will filter only a single entity of the packet
- **Conversation Filter**: Helps you view only the related packets and hide the rest of the packets easily.
- **Colourise Conversation**: Highlights the linked packets without applying a display filter and decreasing the number of viewed packets.
- **Prepare as Filter**: Doesn't apply the filter after the choice is made with right click but instead adds the required query to the pane and waits for the execution command 
- **Apply as Column**: Selecting a line in the packet details you can add columns to the packet list pane. Once you click on a value and apply it as a column, it will be visible on the packet list pane.
- **Follow Stream**: Reconstruct the streams and view the raw traffic as it is presented at the application level. Following the protocol, streams help analysts recreate the application-level data and understand the event of interest (Wireshark automatically creates and applies the required filter to view the specific stream)

### Simple Display Filter Queries
Easiest way to filter quickly the huge amount of packets, is by applying a display filter using the "Apply a display filter" bar
- Protocol Name = `PROTOCOL` (arp, http, dhcp, tcp, imap, ftp)
- Port = `PROTOCOL.port == <port number>`
- IP Address = `ip.addr == 192.168.1.2`

> **Note:** The number of total and displayed packets are always shown on the status bar. 