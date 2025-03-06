# HttpLite

## Contents
1. [Overview](#overview)
2. [Installation](#installation) <br>
  2.1 - [Downloading](#dowloading) <br>
  2.2 - [Build from Source](#build-from-source) <br>
    - [Linux/MacOS](#linuxmacos)<br>
    - [Windows - MinGW](#windows---mingw) 
3. [Include Library in Project](#including-library-in-project)
4. [Methods](#methods)
5. [Usage Examples](#usage-examples)<br>
    - [GET Request](#get-request) <br>
    - [POST Request](#post-request)
6. [Compile and Link](#compile-and-link)
    - [Linux](#on-linux) <br>
    - [Windows - MinGW](#on-windows-using-mingw)
    - [Windows - Visual Studio](#on-windows-using-visual-studio)
7. [Support](#support)


## Overview
HttpLite is a C++ library that is cross platform and light weight. It is designed for making HTTP requests, supporting ``GET`` and ``POST`` requests over ``HTTP`` and ``HTTPS`` using [OpenSSL](https://github.com/openssl/openssl)

## Installation
You can install HttpLite by downloading a precompiled library for your system or building it from source.
### Dowloading
Precompiled binaries are availble [here](https://github.com/flynn-28/HttpLite/releases/tag/release).
The uncompressed folders have the following structure:
```
HttpLite/
├── include/
│   └── HttpLite.h
├── lib/
│   ├── libHttpRequest.a   (Linux and MinGW builds)
│   ├── HttpRequest.lib    (Windows Build -Visual Studio)
```
Copy the include/ and lib/ directories to your project or system include paths.

### Build from Source
#### Linux/MacOS
##### Requirements
* CMake - ``sudo apt install -y cmake``
* Git - ``sudo apt install -y git``
* OpenSSL - ``sudo apt-get install libssl-dev -y``
* g++  - ``sudo apt install g++ -y``
1. Clone repository
```bash
git clone https://github.com/flynn-28/HttpLite && cd HttpLite
```
2. Create Build Directory
```bash
mkdir build && cd build
```
3. Run CMake
```bash
cmake ..
```
4. Make and Install
```bash
make && sudo make install
```
#### Windows - MinGW
##### Requirements
* MinGW
* OpenSSL - [download here](https://slproweb.com/products/Win32OpenSSL.html)
* Git
* Elevated Powershell

1. Clone repository
```bash
git clone https://github.com/flynn-28/HttpLite && cd HttpLite
```
2. Create Build Directory
```bash
mkdir build && cd build
```
3. Run CMake
```bash
cmake .. -G "MinGW Makefiles"
```
4. Make and Install
```bash
mingw32-make
mingw32-make install
```

## Including Library in Project
##### 1. Include the Header
Add the following line to your project:
```c++
#include <HttpLite.h>
```

##### 2. Link the Library
**ASSUMING g++ and MinGW**
* On Linux or MacOS add this to your compile command (assuming extracted from precompiled library):
```
-Iinclude/ -Llib/ -lHttpLite -lssl -lcrypto
```
* On Windows add this to your compile command:
```
-include/ -Llib/ -lHttpLite -lssl -lcrypto -lws2_32
```

## Methods

### Constructors
* `HttpLite()`
        - This initializes the HttpLite object with default values and initializes sockets.

### Destructors
* `~HttpLite()`
        - This uninitializes the HttpLite object and cleans up resources by closing sockets

### Public Methods
* `void setUrl(const std::string& url)`
        - sets request url. accepts string
* `void setMethod(Method method)`
        - sets request method. accepts enum. set to `GET` or `POST`
* `void setPostData(const std::string& data)`
        - sets post request payload. accepts string
* `std::string perform()`
        - performs http request and returns response


### Private Methods
* `void initializeSockets()`
        - initialize sockets
* `void cleanupSockets()`
        - cleans up sockets after use
* `std::string parseHost(const std::string& url)`
        - extract host from url
* `std::string parsePath(const std::string& url)`
        - extract endpoint from url
* `int connectToHost(const std::string& host, int port, bool useSSL)`
        - connects to host
* `std::string buildRequest(const std::string& host, const std::string& path)`
        -  create request based on host, path, method, and content
* `std::string sendRequest(int socket, const std::string& request, bool useSSL)`
        - send request. 
* `std::string extractBody(const std::string& response)`
        - extract body from response based on delimiter

## Usage Examples

### GET Request
```c++
#include <HttpLite.h>
#include <iostream>

int main() {
    try {
        HttpLite request;
        request.setUrl("https://api.msmc.lol/api/color/random");
        request.setMethod(HttpLite::GET);

        std::string response = request.perform();
        std::cout << "Response:\n" << response << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

### POST Request
```c++
#include <HttpLite.h>
#include <iostream>

int main() {
    try {
        HttpLite request;
        request.setUrl("https://httpbin.org/post");
        request.setMethod(HttpLite::POST);
        request.setPostData("key=value&another=123");

        std::string response = request.perform();
        std::cout << "Response:\n" << response << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
```

## Compile *and* Link

#### On Linux
```bash
g++ -o main main.cpp -I<path-to-include-directory> -L<path-to-lib-directory> -lHttpLite -lssl -lcrypto
```
#### On Windows using MinGW
```bash
g++ -o main.exe main.cpp -I<path-to-include-directory> -L<path-to-lib-directory> -lHttpLite -lssl -lcrypto -lws2_32
```
#### On Windows using Visual Studio
1. Add HttpLite.lib and OpenSSL libraries to the project linker input
2. Include the ``include/`` directory in the projects include paths

## Support
For support, submit and issue or pull request, or email me at [msmc.dev@gmail.com](mailto:msmc.dev@gmail.com).

