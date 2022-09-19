<header>
<h1 align="center">
  <a href="https://github.com/jdecorte-be/webserv"><img src=".assets/banner.png" alt="webserv" ></a>
  webserv
  <br>
</h1>

<p align="center">
  A from-scratch HTTP/1.1 web server in C++98. This 42 school project handles concurrent clients with select(), parses requests, and supports CGI for dynamic cont
</p>

<p align="center">
<a href="https://www.42.fr/">
    <img src="https://img.shields.io/badge/School-42-00B8D4?logo=42&logoColor=white&labelColor=000000"
         alt="School 42">
  </a>
<a href="https://github.com/jdecorte-be/webserv">
    <img src="https://img.shields.io/badge/Type-HTTP%2F1.1%20Server-F08D3A?logo=nginx&logoColor=white&labelColor=000000"
         alt="Type HTTP/1.1 Server">
  </a>
<a href="https://isocpp.org/std/the-standard">
    <img src="https://img.shields.io/badge/Standard-C%2B%2B98-00599C?logo=cplusplus&logoColor=white&labelColor=000000"
         alt="Standard C++98">
  </a>
<a href="https://en.wikipedia.org/wiki/I/O_multiplexing">
    <img src="https://img.shields.io/badge/Focus-I%2FO%20Multiplexing-4D76A4?logo=gnubash&logoColor=white&labelColor=000000"
         alt="Focus I/O Multiplexing">
  </a>
</p>

<p align="center">
<a href="https://en.wikipedia.org/wiki/Common_Gateway_Interface">
    <img src="https://img.shields.io/badge/Feature-CGI-33B579?logo=python&logoColor=white&labelColor=000000"
         alt="Feature CGI">
  </a>
  <a href="https://github.com/jdecorte-be/webserv">
    <img src="https://img.shields.io/badge/Type-CLI-informational?logo=terminal&logoColor=white&labelColor=000000&color=66D9EF"
         alt="webserv cli">
  </a>
  <a href="https://github.com/jdecorte-be/webserv">
    <img src="https://img.shields.io/badge/Service-API-blue?logo=fastapi&logoColor=white&labelColor=000000&color=A6E22E"
         alt="webserv api">
  </a>
  <a href="https://github.com/jdecorte-be/webserv/stargazers">
    <img src="https://img.shields.io/github/stars/jdecorte-be/webserv?logo=star&logoColor=white&labelColor=000000&color=E6DB74"
         alt="webserv stars">
  </a>
  <a href="https://github.com/jdecorte-be/webserv/issues">
    <img src="https://img.shields.io/github/issues/jdecorte-be/webserv?logoColor=white&labelColor=000000&color=orange"
         alt="webserv issues">
  </a>
  <a href="https://github.com/jdecorte-be/webserv">
    <img src="https://img.shields.io/github/repo-size/jdecorte-be/webserv?logo=database&logoColor=white&labelColor=000000&color=AE81FF"
         alt="webserv repo size">
  </a>
</p>
<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#usage">Usage</a> •
  <a href="#components">Components</a> •
  <a href="#working-principles">Working Principles</a> •
  <a href="#resources-and-learning">Resources and Learning</a> •
  <a href="#conclusion">Conclusion</a>
</p>
</header>



Webserv is a from-scratch HTTP/1.1 web server implemented in C++98. As a core project for the 42 school curriculum, it is designed to handle multiple client connections concurrently using I/O multiplexing with `select()`. The server can parse HTTP requests, serve static content, and execute CGI scripts to generate dynamic content.

## ✨ Features

*   **HTTP/1.1 Compliance**: Handles `GET`, `POST`, and `DELETE` requests.
*   **C++98 Standard**: Built using only C++98 features and the C standard library, ensuring high portability.
*   **Concurrent Connections**: Utilizes `select()` for non-blocking I/O, allowing it to efficiently manage multiple simultaneous clients.
*   **Custom Configuration**: Server behavior is defined by a flexible configuration file, inspired by NGINX syntax. You can specify ports, server names, error pages, routes, and more.
*   **CGI Support**: Executes CGI scripts (e.g., Python, Perl) to serve dynamic web pages and handle tasks like form submissions and file uploads.
*   **Static File Serving**: Serves various static files, including HTML, CSS, images, and videos.
*   **File Uploads**: Capable of handling file uploads via `POST` requests processed by CGI scripts.
*   **Custom Error Pages**: Allows defining custom pages for different HTTP error codes.
4. **Receive** requests and **send** responses
5. **Close** connections when done

But here's the challenge: How do you handle multiple clients simultaneously without creating a thread for each one?

### Enter I/O Multiplexing with select()

Instead of blocking on a single connection, `select()` allows us to monitor multiple file descriptors (sockets) at once. When data arrives on any socket, `select()` tells us which one is ready, and we process it.

Here's the high-level algorithm:

```cpp
while (true) {
    // Set up file descriptor sets for reading and writing
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Add server socket and all client sockets
    FD_SET(server_socket, &read_fds);
    for (each client_socket) {
        if (has_data_to_read)
            FD_SET(client_socket, &read_fds);
        if (has_data_to_write)
            FD_SET(client_socket, &write_fds);
    }
    
    // Wait for activity on any socket
    select(max_fd + 1, &read_fds, &write_fds, NULL, &timeout);
    
    // Check which sockets are ready
    if (FD_ISSET(server_socket, &read_fds)) {
        // New connection - accept it
        accept_new_client();
    }
    
    for (each client_socket) {
        if (FD_ISSET(client_socket, &read_fds)) {
            // Data available - read and parse request
            read_and_parse_request();
        }
        if (FD_ISSET(client_socket, &write_fds)) {
            // Ready to send - send response
            send_response();
        }
    }
}
```

This non-blocking approach allows a single-threaded server to handle hundreds or thousands of concurrent connections efficiently.

### 2. Request Parser: Decoding HTTP Messages

Parsing HTTP requests is trickier than it seems. You can't just split by newlines and call it a day. HTTP is a streaming protocol—data arrives in chunks, and you need to handle partial requests gracefully.

I implemented a **state machine parser** that processes requests byte-by-byte:

```cpp
enum RequestState {
    REQUEST_METHOD_START,
    REQUEST_METHOD,
    URI_START,
    URI,
    QUERY_STRING_START,
    QUERY_STRING,
    HTTP_VERSION_H,
    HTTP_VERSION_MAJOR,
    HEADER_LINE_START,
    HEADER_KEY,
    HEADER_VALUE,
    POST_BODY,
    CHUNKED_BODY_SIZE,
    // ... and more states
};
```

The parser can be **fed data incrementally**:

```cpp
class Request {
public:
    enum ParseResult {
        PARSE_SUCCESS,
        PARSE_ERROR,
        PARSE_INCOMPLETE
    };
    
    ParseResult feed(const char* data, size_t len);
    
private:
    std::string method;
    std::string uri;
    int versionMajor;
    int versionMinor;
    std::vector<Header> headers;
    std::vector<char> content;
};
```

This design mirrors how production servers like NGINX and Node.js parse HTTP—incrementally and efficiently.

### 3. Response Builder: Crafting HTTP Responses

Once we've parsed the request, we need to build an appropriate response. The `Response` class analyzes the request and determines:

- **Status code**: 200 OK, 404 Not Found, 500 Internal Server Error, etc.
- **Headers**: Content-Type, Content-Length, Connection, etc.
- **Body**: HTML, JSON, file contents, etc.

```cpp
class Response {
private:
    u_short statusCode;
    std::string status;
    std::vector<Header> headers;
    std::vector<char> content;
    
    enum reqStatus {
        LOCATION_NOT_FOUND,
        LOCATION_IS_REDIRECTING,
        METHOD_NOT_ALLOWED,
        REQUEST_TOO_LARGE,
        PATH_NOT_EXISTING,
        PATH_IS_DIRECTORY,
        PATH_IS_FILE,
        OK
    };
    
    reqStatus analyzeRequest(std::string &path);
};
```

The server handles various scenarios:
- **Static files**: Read from disk and serve with appropriate MIME types
- **Directory listing**: Generate HTML directory indexes when autoindex is enabled
- **Redirects**: Send 301/302 responses with Location headers
- **Errors**: Serve custom error pages
- **CGI**: Execute scripts and return their output

### 4. Configuration System: NGINX-Inspired Flexibility

One of my favorite features is the flexible configuration system. Instead of hardcoding server behavior, everything is configurable through a file with NGINX-like syntax:

```nginx
server {
    listen 8080;
    server_name localhost;
    root ./www;
    index index.html;
    client_max_body_size 10M;

    error_page 404 /error/error404.html;

    location / {
        allow_methods GET;
    }

    location /cgi-bin {
        allow_methods GET POST;
        cgi_pass .py /usr/bin/python3;
        cgi_pass .pl /usr/bin/perl;
    }

    location /upload {
        allow_methods POST;
        root ./www/upload;
    }
}
```

This allows you to:
- Host multiple virtual servers on different ports
- Define routes with different behaviors
- Set upload limits and timeouts
- Specify CGI interpreters
- Configure custom error pages

### 5. CGI Support: Dynamic Content Generation

CGI (Common Gateway Interface) is a standard that allows web servers to execute external programs and return their output as HTTP responses. It's how early dynamic websites worked, and it's still useful for certain applications.

When a client requests a CGI script:

1. The server forks a child process
2. Sets up environment variables (REQUEST_METHOD, QUERY_STRING, etc.)
3. Redirects the script's stdout to a pipe
4. Executes the script
5. Reads the output and sends it back to the client

```cpp
// Simplified CGI execution
pid_t pid = fork();
if (pid == 0) {
    // Child process
    setenv("REQUEST_METHOD", request.method.c_str(), 1);
    setenv("QUERY_STRING", request.query_string.c_str(), 1);
    setenv("CONTENT_LENGTH", std::to_string(request.content.size()).c_str(), 1);
    
    dup2(pipe_fd[1], STDOUT_FILENO);
    execve("/usr/bin/python3", argv, envp);
}
// Parent process reads from pipe and sends to client
```

This allows you to write dynamic pages in any language—Python, Perl, Bash, even compiled C++ programs.

## HTTP Methods: GET, POST, DELETE

The server implements the three most common HTTP methods:

### GET: Retrieving Resources

```
GET /page.html HTTP/1.1
Host: localhost
```

The server reads the file from disk and returns it with appropriate headers:

```
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 1234

<html>...</html>
```

### POST: Submitting Data

```
POST /cgi-bin/upload.py HTTP/1.1
Host: localhost
Content-Type: multipart/form-data; boundary=----WebKitFormBoundary
Content-Length: 12345

------WebKitFormBoundary
Content-Disposition: form-data; name="file"; filename="image.jpg"

[binary data]
------WebKitFormBoundary--
```

The server passes the request body to a CGI script, which processes it and returns a response.

### DELETE: Removing Resources

```
DELETE /files/document.pdf HTTP/1.1
Host: localhost
```

The server deletes the file and returns:

```
HTTP/1.1 204 No Content
```

## Handling Edge Cases and Errors

Building a robust server means handling edge cases:

- **Malformed requests**: Return 400 Bad Request
- **Request too large**: Return 413 Payload Too Large
- **Method not allowed**: Return 405 Method Not Allowed
- **File not found**: Return 404 Not Found
- **Server errors**: Return 500 Internal Server Error

The parser validates every byte and can detect:
- Invalid HTTP methods
- Malformed URIs
- Missing required headers
- Incorrect Content-Length
- Invalid chunked encoding

## HTTP Cookies: Maintaining State

HTTP is stateless, but cookies allow servers to maintain user sessions. Here's how it works:

**Server sets a cookie:**
```
HTTP/1.1 200 OK
Set-Cookie: session_id=abc123; Path=/; HttpOnly
```

**Client sends it back:**
```
GET /profile HTTP/1.1
Cookie: session_id=abc123
```

I implemented basic cookie support for session management, allowing features like user authentication and shopping carts.

## Performance Considerations

Some optimizations I implemented:

1. **Non-blocking I/O**: Using `select()` allows handling thousands of connections with a single thread
2. **Keep-Alive connections**: Reusing TCP connections for multiple requests reduces overhead
3. **Efficient parsing**: Byte-by-byte state machine parsing avoids unnecessary string allocations
4. **Static file caching**: Reading files into memory once and serving multiple clients
5. **Timeouts**: Closing idle connections frees up resources

## Challenges and Lessons Learned

### Challenge #1: Partial Reads and Writes

Network I/O is asynchronous. A single `send()` call might not send all your data, and a single `recv()` might return partial data. You need to handle this:

```cpp
// Keep track of how much we've sent
size_t total_sent = 0;
while (total_sent < response.size()) {
    ssize_t sent = send(socket, response.data() + total_sent, 
                       response.size() - total_sent, 0);
    if (sent < 0) {
        // Handle error
    }
    total_sent += sent;
}
```

### Challenge #2: HTTP Chunked Transfer Encoding

Some requests use chunked encoding:

```
POST /upload HTTP/1.1
Transfer-Encoding: chunked

7\r\n
Mozilla\r\n
9\r\n
Developer\r\n
0\r\n
\r\n
```

Each chunk has a size in hexadecimal, followed by the data. The parser needs to handle this incrementally.

### Challenge #3: File Uploads and Multipart Data

Handling `multipart/form-data` for file uploads is complex. You need to parse boundary delimiters and extract file contents while handling them incrementally.

## Testing and Debugging

I used several tools to test and debug:

- **Postman**: Sending custom HTTP requests
- **curl**: Command-line testing
- **Siege**: Load testing with thousands of concurrent connections
- **Wireshark**: Inspecting raw TCP packets
- **Web browsers**: Real-world testing

## 🚀 Getting Started

### Prerequisites

*   A C++ compiler (e.g., `g++`)
*   `make`

### Installation and Execution

1.  **Clone the repository:**
    ```sh
    git clone https://github.com/jdecorte-be/webserv.git
    cd webserv
    ```

2.  **Compile the project:**
    ```sh
    make
    ```

3.  **Run the server:**
    You can run the server with a specific configuration file or use the default one (`nginx.conf`).

    *   **Using a specific configuration:**
        ```sh
        ./webserv nginx.conf
        ```
    *   **Using the default configuration:**
        ```sh
        ./webserv
        ```

Once running, you can access the server by navigating to `http://localhost:<port>` in your web browser, where `<port>` is the port number specified in your configuration file.

## 📁 Project Structure

The repository is organized as follows:

```
.
├── conf/             # Example configuration files
├── server/           # Core server and socket logic
├── parsing/          # HTTP request and configuration parsing
├── cgi/              # CGI handling implementation
├── utils/            # Utility functions
├── www/              # Default web root directory
│   ├── cgi-bin/      # Example CGI scripts
│   ├── error/        # Custom error pages
│   └── ...           # Static assets (HTML, images, etc.)
├── main.cpp          # Main entry point
└── Makefile          # Build script
```

## Conclusion

Building a web server from scratch was one of the most rewarding projects I've completed. It gave me deep insights into:

- How the internet actually works at the protocol level
- Why certain design decisions matter (like non-blocking I/O)
- How production servers like NGINX achieve high performance
- The complexity hiding behind simple HTTP requests

If you're interested in systems programming, networking, or just want to understand the web better, I highly recommend building something like this. The knowledge you gain is invaluable.

### Key Takeaways

1. **HTTP is deceptively simple**: The protocol looks straightforward, but handling edge cases correctly is challenging
2. **I/O multiplexing is powerful**: `select()` allows handling thousands of connections efficiently
3. **State machines are your friend**: Parsing protocols incrementally with state machines is the industry standard
4. **Error handling matters**: A robust server gracefully handles malformed input
5. **Configuration is key**: Making behavior configurable makes your server flexible and reusable

### Resources

If you want to learn more or build your own server, check out these resources:

- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [RFC 9110 - HTTP Semantics](https://www.rfc-editor.org/info/rfc9110)
- [RFC 9112 - HTTP/1.1](https://www.rfc-editor.org/info/rfc9112)
- [HTTP and CGI Explained](https://www.garshol.priv.no/download/text/http-tut.html)
- [NGINX Documentation](https://nginx.org/en/docs/)
