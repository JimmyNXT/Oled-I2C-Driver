#include "SH1106.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH SH1106_LCDWIDTH * SH1106_LCDHEIGHT / 8 

int main() {
  int ret, fd;
  char buffer[BUFFER_LENGTH];

  for(int i = 0; i < sizeof(buffer); i++){
     buffer[i] = 0xAA;
   }

  fd = open("/dev/SH1106_DISPLAY", O_RDWR); // Open the device with read/write access

  if (fd < 0) {
    perror("Failed to open the device...");
    return errno;
  }

  ret = write(fd, buffer, strlen(buffer));

  printf("%d", (int) strlen(buffer));

  if (ret < 0) {
    perror("Failed to write the message to the device.");
    return errno;
  }

  return 0;
}





// int main(){
//   uint8_t mask = 0x01;
//   uint8_t input = 0xAA;
//
//   for (int i = 0; i < 8; i++) {
//     printf("%d: %X\n", i, input & mask);
//     input = input >> 1;
//   }
//
//
//   return 0;
//  }
