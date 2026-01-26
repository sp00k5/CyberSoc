# Burp Suite: The Basics 
> [TryHackMe Link](https://tryhackme.com/room/burpsuitebasics)

Burp Suite is a Java-based framework designed to serve as a comprehensive solution for conducting web application penetration testing. Integrated platform for performing security testing of web applications. Burp Suite captures and enables manipulation of all the HTTP/HTTPS traffic between a browser and a web server providing the ability to intercept, view, and modify web requests before they reach the target server or even manipulate responses before they are received by our browser. Comes in three flavours 
- Community Edition
- Professional
- Enterprise

## Features
Community offers a variety of useful features: 
- **Proxy**: Enables interception and modification of requests and responses while interacting with web applications
- **Repeater**: Allows for capturing, modifying, and resending the same request multiple times (*crafting payloads through trial and error*)
- **Intruder**: Allows for spraying endpoints with requests (*commonly utilized for brute-force attacks or fuzzing endpoints*)
- **Decoder**: Can decode captured information or encode payloads before sending them to the target. 
- **Comparer**: Enables the comparison of two pieces of data at either the word or byte level
- **Sequencer**: Typically employed when assessing the randomness of tokens, such as session cookie values or other supposedly randomly generated data. If the algorithm used for generating these values lacks secure randomness, it can expose avenues for devastating attacks.

> **Note**: Burp Suite facilitates the development of extensions to enhance the framework's functionality. These extensions can be written in Java, Python (using the Java Jython interpreter), or Ruby (using the Java JRuby interpreter). The Burp Suite Extender module allows for loading of extensions into the framework, while the marketplace, known as the BApp Store, enables downloading of third-party modules.

## [Dashboard](https://portswigger.net/burp/documentation/desktop/tools/dashboard)
The dashboard is seperated into quadrants: 

1. **Tasks**: Allows you to define background tasks that Burp Suite will perform while you use the application. In Burp Suite Community, the default “Live Passive Crawl” task, which automatically logs the pages visited
2. **Event log**: Provides information about the actions performed by Burp Suite, such as starting the proxy, as well as details about connections made through Burp.
3. **Issue Activity**: It displays the vulnerabilities identified by the automated scanner, ranked by severity and filterable based on the certainty of the vulnerability.
4. **Advisory**: Provides more detailed information about the identified vulnerabilities, including references and suggested remediations. This information can be exported into a report.

## Navigation
The top menu bars allow you to switch between modules and access various sub-tabs within each module. The sub-tabs appear in a second menu bar directly below the main menu bar. If you prefer to view multiple tabs separately, you can detach them into separate windows. 

There are two types of settings: 
- **Global/User Settings**: These settings affect the entire Burp Suite installation and are applied every time you start the application.
- **Project Settings**: These settings are specific to the current project and apply only during the session (Burp Suite Community Edition does not support saving projects)

## Burp Proxy
