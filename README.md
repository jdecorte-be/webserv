<h1 align="center">
	📖 Webserv
</h1>

<p align="center">
	<b><i>HTTP (Hypertext Transfer Protocol)</i></b><br>
</p>

<p align="center">
	<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/jdecorte-be/42-Webserv?color=lightblue" />
	<img alt="Number of lines of code" src="https://img.shields.io/tokei/lines/github/jdecorte-be/42-Webserv?color=critical" />
	<img alt="Code language count" src="https://img.shields.io/github/languages/count/jdecorte-be/42-Webserv?color=yellow" />
	<img alt="GitHub top language" src="https://img.shields.io/github/languages/top/jdecorte-be/42-Webserv?color=blue" />
	<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/jdecorte-be/42-Webserv?color=green" />
</p>

The "Webserv" project, a part of the 42 school's core curriculum, was collaboratively developed by Anastasiia-Ni and AhmadMHammoudeh. This project involves building a web server compatible with C++98 from the ground up. It's designed to handle HTTP requests such as GET, HEAD, POST, PUT, and DELETE, and can serve both static and dynamic content. The server is capable of handling multiple client connections concurrently using the select() method.

## Key Features
- **HTTP Request Handling:** Processes different types of HTTP requests and serves static or dynamic content.
- **Concurrent Connections:** Utilizes select() for managing multiple client connections.
- **Custom Configuration:** Allows specifying server settings via a configuration file.
- **CGI Support:** Integrates CGI for dynamic content generation.

## Usage
To use the server:
```bash
make
./webserv [Config File] # Default configuration used if left empty.
```

## Components
- **Server Core:** Manages TCP connections, sockets, and data flow.
- **Request Parser:** Interprets HTTP requests, extracting methods, paths, headers, and bodies.
- **Response Builder:** Constructs HTTP responses with appropriate headers and content.
- **Configuration File:** Allows customization of server operations and settings.
- **CGI Integration:** Facilitates dynamic content generation through external scripts.

## Working Principles
- **HTTP Basics:** The server operates on standard HTTP protocols, processing requests and responses.
- **I/O Multiplexing:** Utilizes I/O multiplexing for efficient handling of multiple requests.
- **Error Handling:** Capable of identifying and responding to various request errors.

## Resources and Learning
- **Networking and HTTP Guides:** Various resources for understanding HTTP server fundamentals and networking concepts.
- **RFC References:** Links to relevant RFCs for in-depth protocol knowledge.
- **CGI Tutorials:** Learning resources for CGI implementation in web programming.
- **Support Tools:** Tools like Postman and Wireshark recommended for testing and analysis.
- **Additional References:** Links to StackOverflow discussions, encoding guides, and other useful resources.

## Conclusion
This project encapsulates the essentials of a functional HTTP server, providing a comprehensive understanding of web server architecture, HTTP protocols, networking, and server-side scripting. It's a hands-on approach to learning the intricacies of web server development.
