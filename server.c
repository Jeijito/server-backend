#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>  // SOCKET API
#include <netinet/in.h>  // INTERNET PROTOCOL
#include <unistd.h>      // POSIX API
#include "config.h"	// config for private stuff

int main() {
    int server_fd; 
    int client_fd; // To store the dedicated client communication channel
    
    struct sockaddr_in address;
    int port = SERVER_PORT;

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

    //Temporary measure to keep the connection open
    printf("Holding channel open for 2 seconds...\n");
    sleep(2);

    // Allocate a small 1-byte buffer memory space to read the incoming command opcode
    unsigned char command_buffer;
    ssize_t bytes_read;

    printf("Awaiting command byte from client...\n");

    // Read exactly 1 byte from the client communication pipe
    bytes_read = read(client_fd, &command_buffer, 1);

    if (bytes_read < 0) {
        perror("Read operation failed");
    } 
    else if (bytes_read == 0) {
        printf("Client disconnected before sending a command.\n");
    } 
    else {
        // Evaluate the command using our protocol definitions
        switch (command_buffer) {
            case 0x01:
                printf("[PROTOCOL SUCCESS] Received Command 0x01: UPLOAD REQUEST triggered.\n");
                // Future implementation: handle_upload(client_fd);
                break;
            case 0x02:
                printf("[PROTOCOL SUCCESS] Received Command 0x02: DOWNLOAD REQUEST triggered.\n");
                // Future implementation: handle_download(client_fd);
                break;
            default:
                printf("[PROTOCOL WARNING] Received Unknown Command: 0x%02X. Terminating session.\n", command_buffer);
                break;
        }
    }


    // 6. Orderly execution teardown (Close client connection, then release the master listener)
    close(client_fd);
    close(server_fd);
    
    printf("Server shut down cleanly.\n");
    return 0;
}
