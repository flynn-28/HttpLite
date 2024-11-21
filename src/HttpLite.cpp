#include "HttpLite.h"
#include <sstream>
#include <cstring>
#include <openssl/ssl.h>
#include <openssl/err.h>

HttpLite::HttpLite() : method(GET), url(""), postData("") {
    initializeSockets();
}

HttpLite::~HttpLite() {
    cleanupSockets();
}

void HttpLite::initializeSockets() {
#if defined(_WIN32) || defined(_WIN64)
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

void HttpLite::cleanupSockets() {
#if defined(_WIN32) || defined(_WIN64)
    WSACleanup();
#endif
}

void HttpLite::setUrl(const std::string& url) {
    this->url = url;
}

void HttpLite::setMethod(Method method) {
    this->method = method;
}

void HttpLite::setPostData(const std::string& data) {
    this->postData = data;
}

std::string HttpLite::parseHost(const std::string& url) {
    size_t start = url.find("://") + 3;
    size_t end = url.find("/", start);
    return url.substr(start, end - start);
}

std::string HttpLite::parsePath(const std::string& url) {
    size_t start = url.find("://") + 3;
    size_t pathStart = url.find("/", start);
    return (pathStart == std::string::npos) ? "/" : url.substr(pathStart);
}

int HttpLite::connectToHost(const std::string& host, int port, bool useSSL) {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res) != 0) {
        throw std::runtime_error("Failed to resolve host.");
    }

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0 || connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        freeaddrinfo(res);
#if defined(_WIN32) || defined(_WIN64)
        closesocket(sock);
#else
        close(sock);
#endif
        throw std::runtime_error("Failed to connect.");
    }

    freeaddrinfo(res);
    return sock;
}

std::string HttpLite::buildRequest(const std::string& host, const std::string& path) {
    std::ostringstream request;
    request << (method == GET ? "GET" : "POST") << " " << path << " HTTP/1.1\r\n";
    request << "Host: " << host << "\r\n";
    if (method == POST) {
        request << "Content-Length: " << postData.size() << "\r\n";
        request << "Content-Type: application/x-www-form-urlencoded\r\n";
    }
    request << "Connection: close\r\n\r\n";
    if (method == POST) {
        request << postData;
    }
    return request.str();
}

std::string HttpLite::sendRequest(int socket, const std::string& request, bool useSSL) {
    SSL_CTX* ctx = nullptr;
    SSL* ssl = nullptr;

    if (useSSL) {
        SSL_library_init();
        ctx = SSL_CTX_new(SSLv23_client_method());
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, socket);

        if (SSL_connect(ssl) <= 0) {
            SSL_free(ssl);
            SSL_CTX_free(ctx);
            throw std::runtime_error("Failed to establish SSL connection.");
        }

        SSL_write(ssl, request.c_str(), request.size());
        char buffer[8192];
        int bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        buffer[bytes] = '\0';

        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return std::string(buffer);
    } else {
        send(socket, request.c_str(), request.size(), 0);
        char buffer[8192];
#if defined(_WIN32) || defined(_WIN64)
        int bytes = recv(socket, buffer, sizeof(buffer) - 1, 0);
#else
        int bytes = read(socket, buffer, sizeof(buffer) - 1);
#endif
        buffer[bytes] = '\0';
        return std::string(buffer);
    }
}

std::string HttpLite::extractBody(const std::string& response) {
    size_t headerEnd = response.find("\r\n\r\n");
    return (headerEnd != std::string::npos) ? response.substr(headerEnd + 4) : "";
}

std::string HttpLite::perform() {
    bool useSSL = url.substr(0, 5) == "https";
    std::string host = parseHost(url);
    std::string path = parsePath(url);
    int port = useSSL ? 443 : 80;

    int sock = connectToHost(host, port, useSSL);
    std::string request = buildRequest(host, path);
    std::string response = sendRequest(sock, request, useSSL);

#if defined(_WIN32) || defined(_WIN64)
    closesocket(sock);
#else
    close(sock);
#endif

    return extractBody(response);
}
