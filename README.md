Goal: Make an HTTP server

HTTP Server in progress:
- 


TCP Server completed:
- Working server that accepts client request and ignores (obvious) bad requests without crashing
- client can send requests to communicate with the server and choose one of two meals!
- Communication currently only happens through localhost, will try LAN after http server
- Problem: Client communication is hard-coded and the client might now handle an unpredictable event properly    
One solution could be that the server sends a signal, indicating if the server response is good or bad, if good, the client can carry on with it's 'script', if bad then display some message and rerun that logic.  
I will skip this part for now and wait to handle this when I've implemented an HTTP server instead of making my custom signals now


Process of making the HTTP server:  
In my toy server, the server sends the first message, but in HTTP, the client send()s the first request which the server recv()s, parses until CRLF (`\r\n\r\n`) and then goes about doing it's business

Short goals:
1) [DONE] Make the client send the first request, and have the server recv it.  
2) Implement a while loop in the client and break it after the final response is received
3) Make the first client request, an HTTP request 'GET /index HTTP/1.1\r\n' and have the server parse it.  
4) Start implementing the routing mechanism. Make index.html containing "`<html>Welcome!</html><br>`" and route to it.  
5) 


Process of making the TCP server:  
The server connects to the client and gets out of the accept() 'block' once the client
initiates the 'connect()' call

The server will only move on and 'enter' accept() once the three way handshake is complete

How does the content actually get rendered? Who is doing the rendering?

Ans: The browser! Here’s the flow:

- enter a URL (e.g. google.com, 127.0.0.1, etc.).
- The browser acts as a client and connects to the server.
- The browser sends a request (usually GET /index.html HTTP/1.1), asking for an HTML file.
- The server replies with an HTTP response like:
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 1024
...

`<html>Hello World</html>`


- The browser sees the Content-Type (text/html) and hands the body (`<html>...</html>)` to the HTML parser.
- The parser builds a DOM tree (Document Object Model):

```
HTML
 └── BODY
      └── Hello World
```


- The browser also loads CSS (if any) and builds a CSSOM tree (CSS Object Model).
- DOM + CSSOM are combined into a Render Tree.
- This decides what elements actually appear on screen and with what styles.
- The rendering engine (Blink, Gecko, WebKit, etc.) runs layout (figuring out positions, sizes) and then painting (turning nodes into pixels).
- The browser’s graphics layer (often backed by OpenGL, Vulkan, or DirectX) draws the final pixels on the screen!

text/plain
If instead of text/html, the server responds with plain text, then the html is not parsed, and the return would look something like this:
```<html>Hello World</html>```

Pure text, no rendering, no styling!



Next project: Chat Server