#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <endian.h>
#include "upload.h"

void handle_upload(int client_socket) {
    printf("[UPLOAD ENGINE] Initializing file reception pipeline...\n");

    uint8_t filename_len = 0;
    uint64_t file_size = 0;
    ssize_t bytes_received;

    // Read exactly 1 byte to discover filename_len

    bytes_received = recv(client_socket, &filename_len, 1, 0);
    
    if (bytes_received <= 0) {
        perror("[UPLOAD ERROR] Failed to read filename length");
        return;
    }

    printf("[UPLOAD DECODE] Filename length expected: %u bytes\n", filename_len);
    // Read exactly 'filename_len' bytes to capture filename string

    char filename[256]; 
    bytes_received = recv(client_socket, filename, filename_len, 0);
    if (bytes_received <= 0) {
        perror("[UPLOAD ERROR] Failed to read filename string");
        return;
    }
    filename[filename_len] = '\0'; // Manually null-terminate the string safely
    printf("[UPLOAD DECODE] Target filename: %s\n", filename);


    // Read exactly 8 bytes to capture the file_size integer
    bytes_received = recv(client_socket, &file_size, 8, 0);
    if (bytes_received <= 0) {
        perror("[UPLOAD ERROR] Failed to read file size");
        return;
    }

    // Convert from Network Byte Order (Big-Endian) to Host Byte Order (CPU-Native)
    file_size = be64toh(file_size);

    printf("[UPLOAD DECODE] Total file footprint expected: %lu bytes\n", file_size);


    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("[UPLOAD ERROR] Failed to create local file on disk");
        return;
    }
    printf("[STORAGE ENGINE] Target disk track created successfully: %s\n", filename);

    // 4. Next step concept: Enter while loop writing to disk until file_size is hit

    char buffer[4096];
    uint64_t total_bytes_written = 0;
    size_t chunk_to_read;

    while (total_bytes_written < file_size) {
        // Determine how many bytes we should look for next to avoid over-reading
        uint64_t remaining_bytes = file_size - total_bytes_written;
        if (remaining_bytes < sizeof(buffer)) {
            chunk_to_read = remaining_bytes;
        } 
        
        else {
            chunk_to_read = sizeof(buffer);
        }

        // Pull a chunk off the wire
        bytes_received = recv(client_socket, buffer, chunk_to_read, 0);
        if (bytes_received < 0) {
            perror("[UPLOAD ERROR] Network stream interrupted unexpectedly");
            break;
        } 
        
        else if (bytes_received == 0) {
            printf("[UPLOAD WARNING] Client hung up before full transmission completed.\n");
            break;
        }

        // Safely write the exact chunk footprint directly onto the physical drive
        size_t bytes_written = fwrite(buffer, 1, bytes_received, fp);
        if (bytes_written < bytes_received) {
            perror("[STORAGE ERROR] Disk write error or drive full");
            break;
        }

        total_bytes_written += bytes_written;
    }

    // 6. Close the local disk stream file descriptor and secure the data
    fclose(fp);
    printf("[UPLOAD SUCCESS] File sync finalized. Bytes written to storage track: %lu\n", total_bytes_written);

}