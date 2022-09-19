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


![Webserv Banner](.assets/banner.png)

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

## ⚙️ Configuration

The server is configured using a file with a syntax similar to NGINX. A `server` block defines a virtual server, and `location` blocks define how to handle requests for different URIs.

### Example Configuration

Here is a basic example of a server configuration:

```nginx
server {
    listen              8080;
    server_name         localhost;
    root                ./www;
    index               index.html;
    client_max_body_size 10M;

    error_page 404 /error/error404.html;

    location / {
        allow_methods GET;
    }

    location /cgi-bin {
        allow_methods GET POST;
        cgi_pass .py /usr/bin/python; # Execute .py files with the python interpreter
        cgi_pass .pl /usr/bin/perl;   # Execute .pl files with the perl interpreter
    }

    location /upload {
        allow_methods POST;
        root ./www/upload;
    }
}
```

See the `nginx.conf` file and the examples in the `conf/` directory for more details.

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

## 🤝 Acknowledgements

This project was collaboratively developed by:

*   **Anastasiia-Ni** ([@Anastasiia-Ni](https://github.com/Anastasiia-Ni))
*   **AhmadMHammoudeh** ([@AhmadMHammoudeh](https://github.com/AhmadMHammoudeh))
