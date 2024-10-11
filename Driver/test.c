#include "SH1106.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH SH1106_LCDWIDTH * SH1106_LCDHEIGHT / 8 

int main() {
  int ret, fd;
  char buffer[BUFFER_LENGTH];

  for(int i = 0; i < sizeof(buffer); i++){
     buffer[i] = 0x10;
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
//
//
//
//
// #include <stdio.h>
// #include <stdlib.h>
//
// int main(){
//   char buf[SH1106_LCDWIDTH * SH1106_LCDHEIGHT / 8];
//
//   for(int i = 0; i < sizeof(buf); i++){
//     buf[i] = 0xAA;
//   }
//
//   FILE *fptr = fopen("/dev/SH1106_DISPLAY", "w");
//   // fprintf(fptr, "%.*s", (int) sizeof(buf), buf);
//   fprintf(fptr, "%s", "This is a test\0");
//
//   fclose(fptr);
//
//
//   return 0;
//  }
