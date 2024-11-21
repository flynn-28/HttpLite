#ifndef HTTPLITE_H
#define HTTPLITE_H

#include <string>
#include <map>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

class HttpLite {
public:
    enum Method { GET, POST };

    HttpLite();
    ~HttpLite();

    void setUrl(const std::string& url);
    void setMethod(Method method);
    void setPostData(const std::string& data);
    std::string perform();

private:
    std::string url;
    Method method;
    std::string postData;

    void initializeSockets();
    void cleanupSockets();
    std::string parseHost(const std::string& url);
    std::string parsePath(const std::string& url);
    int connectToHost(const std::string& host, int port, bool useSSL);
    std::string buildRequest(const std::string& host, const std::string& path);
    std::string sendRequest(int socket, const std::string& request, bool useSSL);
    std::string extractBody(const std::string& response);
};

#endif
