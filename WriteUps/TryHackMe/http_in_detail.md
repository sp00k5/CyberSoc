# HTTP in Detail
> [TryHackMe Link](https://tryhackme.com/room/httpindetail)

HTTP (HyperText Transfer Protocol) - A set of rules used for communicating with web servers for transmitting of webpage data. HTTPS is the Secure version of this by encrypting the data

URL (Uniform Resource Locator) - Is a instruction on where / how to access a resource on the internet. It is made up of: 

- **Scheme**: This instructs on what protocol to use for accessing the resource such as , HTTPS, FTP
- **User**: Some services require authentication to log in, you can put a username and password into the URL to log in.
- **Host**: The domain name or IP address of the server you wish to access (subdomain can go at the start of this).
- **Port**: The Port that you are going to connect to, usually 80 for and 443 for HTTPS, but this can be hosted on any port between 1 - 65535.
- **Path**: The file name or location of the resource you are trying to access.
- **Query String**: Extra bits of information that can be sent to the requested path. For example, /blog?id=1 would tell the blog path that you wish to receive the blog article with the id of 1.
- **Fragment**: This is a reference to a location on the actual page requested. This is commonly used for pages with long content and can have a certain part of the page directly linked to it, so it is viewable to the user as soon as they access the page.

## Making a Request 
Request Method + HTTP Protocol Version -> GET / HTTP/1.1 <- Page requested 

```
GET / HTTP/1.1 

Host: tryhackme.com
User-Agent: Mozilla/5.0 Firefox/87.0
Referer: https://tryhackme.com/

```
1. GET method, request the home page with / and telling the web server we are using protocol version 1.1.
2. Tell the web server we want the website tryhackme.com
3. Tell the web server we are using the Firefox version 87 Browser
4. Telling the web server that the web page that referred us to this one is https://tryhackme.com 
5. HTTP requests always end with a blank line to inform the web server that the request has finished.
   
### Receiving a Response

```
HTTP/1.1 200 OK

Server: nginx/1.15.8
Date: Fri, 09 Apr 2021 13:34:03 GMT
Content-Type: text/html
Content-Length: 98

<HTML CONTENT>
```

1. HTTP 1.1 is the version of the protocol the server is using and then followed by the Status Code (200 OK)
2. Web Server software and version number
3. Current Date,time and timezone of the web server
4. Content-Type header tells the client what sort of information is going to be sent (HTML, images, videos, pdf, XML)
5. Content-Length tells the client how long the response is, can confirm no data is missing
6. HTTP response contains a blank line to confirm the end of the response
7. The information that has been requested, in this instance the homepage.

HTTP methods are a way for the client to show their intended action when making an HTTP request. 
- **GET**:  Used for getting information from a web server.
- **POST**: Used for submitting data to the web server and potentially creating new records
- **PUT**: Used for submitting data to a web server to update information
- **DELETE**: Used for deleting information/records from a web server

A [status code](https://http.cat/) informs the client of the outcome of their request and also potentially how to handle it.

| Code | Description |
| ---- | ----------- | 
| 100-199 - Information Response | These are sent to tell the client the first part of their request has been accepted and they should continue sending the
of their request. These codes are no longer very common. | 
| 200-299 - Success   |	This range of status codes is used to tell the client their request was successful. | 
| 300-399 - Redirection |	These are used to redirect the client's request to another resource. This can be either to a different webpage or a different website altogether. | 
| 400-499 - Client Errors | 	Used to inform the client that there was an error with their request. | 
| 500-599 - Server Errors |  	This is reserved for errors happening on the server-side and usually indicate quite a major problem with the server handling the request. | 

Cookies are saved when you receive a "Set-Cookie" header from a web server.