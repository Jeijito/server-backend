#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>  // SOCKET API
#include <netinet/in.h>  // INTERNET PROTOCOL
#include <unistd.h>      // POSIX API

int main() {
    int server_fd; 
    int client_fd; // To store the dedicated client communication channel
    
    struct sockaddr_in address;
    int port = 8080;

    // 1. Create the master network socket (IPv4, TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("SOCKET CREATION FAILED");
        exit(EXIT_FAILURE);
    }

    printf("Socket created FD ID: %d\n", server_fd);

    // 2. Clear and format the address structure layout
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons(port);  

    // 3. Bind the socket to our physical interface and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind operation failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Socket successfully bound to port %d!\n", port);

    // 4. Put the socket into listening mode with a backlog queue threshold of 3
    if (listen(server_fd, 3) < 0) {
        perror("Listen operation failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is now actively listening on port %d... Waiting for connections...\n", port);

    // 5. Instantiating memory spaces to hold incoming client identification metadata
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);

    // This call blocks execution until a client knocks on port 8080
    client_fd = accept(server_fd, (struct sockaddr *)&client_address, &addrlen);

    if (client_fd < 0) {
        perror("Accept operation failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted! Client connected via File Descriptor ID: %d\n", client_fd);

    // 6. Orderly execution teardown (Close client connection, then release the master listener)
    close(client_fd);
    close(server_fd);
    
    printf("Server shut down cleanly.\n");
    return 0;
}
