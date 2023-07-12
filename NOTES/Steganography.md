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
- Appending bits to the end of carrier file
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
  

## SLIDE (5)
`Cryptanalysis`

## SLIDE (6)
`Anti-Forensics`

## SLIDE (7)
`Tools`
