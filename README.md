Goal: Make an HTTP server

Start by making a TCP server

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