## Steganography
<div align="center">
HIDDEN IN PLAIN SIGHT
</div>

------

## SLIDE (1)
`Stegaography`

Is the *"art through which writing is hidden requiring recovery by minds of men"*. Although this hiding of messages requires a rule-set that is strictly maintained.

**Bible Code** - Michael Drosnin's code of using the bible to prove things happened when the rulse are constantly changing and are broad means anything can be determined. **THIS IS NOT STEGANOGRAPHY**

### Modern Steganography
The practice of concealing information within another message or physical object to avoid detection. Steganography can be used to hide virtually any type of digital content, including text, image, video, or audio content. That hidden data is then extracted at its destination. This data is encrypted (prior to hiding) making header discovery difficult and evidence collection challenging. Process:
- Data is encrypted
- Data is then inserted and hidden (special algorithms which add/modify contents of carrier)
- Data can be appended or placed throughout

> The best carrier files are larger in size and contain other formats (bmp,jpg,gif)

Uses of Steganography:
- Corporate Espionage
- Anti-Forensic mechanisms (mitigating effectiveness of forensic investigations)
- Hiden Communication

**Kerckhoff's Principle** - A cryptographic system should be secure even if everything about the system, except for the key, is public knowledge.

This is also true in steganography where to remain undetected:
- The unmodified cover medium must maintain secret (if exposed a comparison can be made between the original and steg)
- The application used for transformation and its algorithms for processing the image should be hidden

**Carrier Files**:
| File Type | Advantage |
|---------|-----|
| BMP | Large file size with excess data |
| JPEG | Good due to how common they are, (bad with compression) |
| GIF | Common and large file size|
| MP3 | Hide audio layers inside original carrier |

### Advantages
- hiding communications so well that they receive no attention
- A form of encryption that protects the information within a message and the connections between sender and receiver
- three essential elements of steganography—security, capacity, and robustness—make it worthwhile to covert information transfer
- store an encrypted copy of a file containing sensitive information

## SLIDE (2 / 3)
`Hiddign Techniques`

Steganography varies with its approaches and each program can use a different algorithm to hiding the data. Without knowing which tool was used then it is pretty much impossible to identify existence of hidden data. Meaning sometimes its better to find existence of the `original carrier file` / `steg hiding program` / `existence of message rather than extraction` 

**Techniques include**:
- Hiding data in bits that represent the same color pixels repeated in a row in an image file
- Appending bits to the end of carrier file (magic numbers)
- Hidden inside unused header portions of a carrier
- Least Significant Bit (LSB)
  -   Changing the furtherest right bit in each byte to then be collected and form the hidden data
- Bit Plane Complexity Segmentation
    - A method of embedding a message file in an image through replacing the most "complex" regions with the data from the message file
    - Hidden in pixel blocks of all the planes, which have noisy patterns in them [info](https://www.hindawi.com/journals/am/2015/698492/)
    - Used in Watermarks
- Interlacing
  - Creating a bitmap of pixel encoding and hiding another image within the pixels of the carrier
- Stochastic Modulation
  - Embeds secret message within the cover image by adding stego-noise with a specific probabilistic distribution to divert suspicion  

## SLIDE (4)
`Steganalysis`

Detecting the hidden content and find evidence to prove the existence of the message much rather than extraction
- Find tool used in registry
- Find original carrier in recycling bin

**Detection Methods**:
- Visual Detection : Corruption / Discoloration / Pixelation
- Audible Detection : echo / double track
- Statistical Detection : Histogram / Luminence graph / Hex editor
- Structural Detection : File size diference / date & time difference / checksum / hash / meta-data

Programs which perform steganography usually leave particular traces of patterns that can aid steganalysis, known as **signatures**. Being able to identify the  existence of it you are able to presenbt evidence of steganography and possibly discover the program used to hide the data. These methods of detection must be:
- Accurate
- Consistent
- Minimize false-positives

**Anomaly Analysis** - Comparing the properties of the files looking at the file size and comparing to a copy of the original if possible.

**Signature-Based Analysis** - Analysing the contents of the hex dump or raw contents of the document to view signatures from tools used. Tools such as Hiderman will add a signature to the end of the document replacing the magic numbers as they are used by the program when extracting the data. 

## SLIDE (5)
`Cryptanalysis`

This is when messages are encrypted and then hidden, highly common form of anti-forensic as noise and message are undistinguishable. The algorithms commonly used are based on symmetric key cryptography. Being able to crack the algorithm or break the encryption is practically impossible to achieve with modern encryption in the time prohibited for the data to be relevant. However crtpyanalysis again aims to focus on identifying the existence rather than extraction:
- Identify program used to hide message
- Identify location of program signature
- Identify location of the password
- Identify location of hidden message
- Identify algorithym used to encrypt

Common encryption techniques deployed alongside steganography are:
- Algorithms based on secret key
- Algorithms based on a password
- Algorithms based on random seed

## SLIDE (6)
`Anti-Forensics`

Anti-forensic techniques are actions whose goal is to prevent the proper investigation process or make it much harder. These actions are aimed at reducing the quality and quantity of digital evidence. These are deliberate actions of not only computer users but also of developers who write programs secured prior to the methods of Cyber forensics.
- Avoiding detection of compromising events that have taken place.
-  Disrupting and preventing the collection of information.
- Increasing the time that an examiner needs to spend on a case.
- Casting doubt on a forensic report or testimony.
- Subverting the forensic tool (for example, using the forensic tool itself to attack the organization in which it is running).
- Leaving no evidence that an anti-forensic tool has been run.

Approaches within steganography to avoid message recovery include:
- Delete original message
- Obfuscation and encryption
- Remove the Steganography program
- Run the Steganography program from a Live Boot / Disk Wiping
- Microwave RAM

### Alternate Data Streams
One file can link data to many alternate data stream directions with any file sizes, hiding files and directories making them difficult to detect. NTFS allows this fork / stream of files and folders:
- Accessed only through the original file
- Can’t be seen with C:/dir
- MakeStream software which can move malware to datastreams

> Malware may try to hide in the NTFS stream after the malware has already infected a system. The stream portion of a file is lost during FTP downloads so malware does not use it for distribution

## SLIDE (7)
`Tools`

| Detection | Creation |
|---------|-----|
| Binwalk | DarkCryptTC |
| XDD | OpenPuff |
| Hexdump | StegoShare |
| StegSpy | StegFS |
| Stegbreak | HiderMan |
| Knoppix | Jsteg | 
| LNS | MP3stego |
| LADS | Paranoid |
| NTFS ADS Check | [More Tools](http://www.jjtc.com/Security/stegtools.htm) |
