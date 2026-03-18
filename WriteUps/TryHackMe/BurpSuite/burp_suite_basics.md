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
Enables the capture of requests and responses between the user and the target web server. This intercepted traffic can be manipulated, sent to other tools for further processing, or explicitly allowed to continue to its destination.
- **Intercepting Requests**: Allow for further actions such as forwarding, dropping, editing, or sending them to other Burp modules.
- Provides control for web traffic
- **Capture and Logging**: Captures and logs requests made through the proxy by default (even when the interception is turned off). Captured requests can be viewed in the HTTP history and WebSockets history sub-tabs
- **WebSocket Support**: Captures and logs WebSocket communication

### Features 
- **Response Interception**: By default, the proxy does not intercept server responses unless explicitly requested on a per-request basis. The "Intercept responses based on the following rules" checkbox, along with the defined rules, allows for a more flexible response interception.
- **Match and Replace**: The "Match and Replace" section in the Proxy settings enables the use of regular expressions (regex) to modify incoming and outgoing requests. This feature allows for dynamic changes, such as modifying the user agent or manipulating cookies.
-  Burp Suite also includes a built-in Chromium browser that is pre-configured to use the proxy without any of the modifications we just had to do.


## Target Tab
Provides us with three views:

### Site map
Allows us to map out the web applications we are targeting in a tree structure. Every page that we visit while the proxy is active will be displayed on the site map. 
- Automatically generate a site map by simply browsing the web application.  
- Burp Suite Professional the site map can perform automated crawling of the target 
- API endpoints are also captured

### Issue definitions
Access to a list of all the vulnerabilities that the scanner looks for. The Issue definitions section provides an extensive list of web vulnerabilities, complete with descriptions and references.

### Scope settings
Allows us to control the target scope in Burp Suite and it enables us to include or exclude specific domains/IPs to define the scope of our testing
- However, even if we disabled logging for out-of-scope traffic, the proxy will still intercept everything. To prevent this, we need to go to the Proxy settings sub-tab and select **And URL Is** in target scope from the "Intercept Client Requests" section.


> **Note**: When intercepting HTTP traffic encountering an issue when navigating to sites with TLS enabled will read indicating that the PortSwigger Certificate Authority (CA) is not authorised to secure the connection. To fix this navigate to http://burp/cert. This will download a file called cacert.der and upload it to the browser