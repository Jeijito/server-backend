#include <stdio.h>
#include <stdint.h>      // Provides explicit byte-width types like uint8_t, uint64_t
#include <sys/socket.h>  // Provides the read/recv system functions
#include "upload.h"

void handle_upload(int client_socket) {
    printf("[UPLOAD ENGINE] Initializing file reception pipeline...\n");

    uint8_t filename_len = 0;
    uint64_t file_size = 0;

    // 1. Next step concept: Read exactly 1 byte to discover filename_len
    // 2. Next step concept: Read exactly 'filename_len' bytes to capture filename string
    // 3. Next step concept: Read exactly 8 bytes to capture the file_size integer
    // 4. Next step concept: Enter while loop writing to disk until file_size is hit
}