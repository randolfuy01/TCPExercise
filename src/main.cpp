#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>

std::string getLocalIP();

int main() {
    std::cout << "TCP server starting..." << std::endl;

    // Creating the server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Define the server address
    sockaddr_in address;
    address.sin_family = AF_INET;

    // Conver the unsigned int from machine byte order to network byte order
    int port = 8080;
    address.sin_port = htons(port);

    // Make our socket listen to all available IPs
    address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket using bind()
    bind(serverSocket, (struct sockaddr*)&address, sizeof(address));

    std::cout << "Server listening on port: " << port << std::endl;

    // Get IP 
    std::string ip = getLocalIP();
    std::cout << "Server local IP address: " << ip << std::endl;
    listen(serverSocket, 5);
    int clientSocket = accept(serverSocket, nullptr, nullptr);

    // Buffer for the message
    char buffer[1024] = { 0 };

    while (true) {
        recv(clientSocket, buffer, sizeof(buffer), 0);
        if (strlen(buffer) == 0) break;
        std::cout << "Message from client: " << buffer << std::endl; 
        
        std::string response = "Hello from the server\n";
        send(clientSocket, &response, response.length(), 0);
    }

    std::cout << "Shutting down server socket" << std::endl;
    
    close(serverSocket);

    return 0;
}

std::string getLocalIP() {
    
    char buffer[256];
    struct sockaddr_in sa;

    if (gethostname(buffer, sizeof(buffer)) == 0) {
        struct hostent* host = gethostbyname(buffer);
        if (host != nullptr) {
            struct in_addr* address = (struct in_addr*)host->h_addr_list[0];
            return inet_ntoa(*address);
        }
    }
    return "";
}