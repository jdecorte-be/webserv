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
