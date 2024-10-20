// #include "SH1106.h"
#include "SH1106_IOCTL.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>

int xyToIndex(int x, int y) { return x + (y * 128); }
// int main() {
//   int ret, fd, ans;
//   struct screen_size pScreenSize;
//   fd = open("/dev/SH1106_DISPLAY", O_RDWR); // Open the device with read/write access
//
//   if (fd < 0) {
//     perror("Failed to open the device...");
//     return errno;
//   }
//
//   ioctl(fd, GET_WIDTH, &ans);
//
//   printf("Width is: %d\n", ans);
//
//
//   ioctl(fd, GET_HEIGHT, &ans);
//
//   printf("Height is: %d\n", ans);
//
//   ioctl(fd, GET_SIZE, &pScreenSize);
//
//   printf("Width is: %d and Height is: %d\n", pScreenSize.width, pScreenSize.height);
//   close(fd);
//
//   return 0;
// }


int main() {
  int ret, fd, width, height, buffer_length;
  fd = open("/dev/SH1106_DISPLAY", O_RDWR); // Open the device with read/write access

  if (fd < 0) {
    perror("Failed to open the device...");
    return errno;
  }
  ioctl(fd, GET_WIDTH, &width);
  ioctl(fd, GET_HEIGHT, &height);

  buffer_length = width * height / 8;

  uint8_t buffer[buffer_length];

  for(int i = 0; i < buffer_length; i++){
     buffer[i] = 0xAA;
  }

  bool bool_buffer[width * height];
  bool temp_bool_buffer[width * height];

  for (int i = 0; i < (width * height); i++) {
    bool_buffer[i] = false;
    temp_bool_buffer[i] = false;
  }

  int index = 0;
  uint8_t mask = 0x01;

  for (int i = 0; i < buffer_length; i++) {
    uint8_t c = buffer[i];

    for (int j = 0; j < 8; j++) {
      uint8_t temp_c = c & mask;
      if(temp_c == 0x01){
        bool_buffer[index] = true;
      }
      c = c >> 1;
      index++;
    }
  }

  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      if(bool_buffer[xyToIndex(x, y)]){
        printf("1 ");
      } else {
        printf("0 ");
      }
    }
    printf("\n");
  }

  for (int x = 0; x < width; x = x + 8) {
    for (int y = 0; y < height; y = y + 8) {
      for (int xoff = 0; xoff < 8; xoff++) {
        for (int yoff = 0; yoff < 8; yoff++) {
          temp_bool_buffer[xyToIndex(x + xoff, y + yoff)] = bool_buffer[xyToIndex(x + yoff, y + xoff)];
        }
      }
    }
  }

  index = 0;

  for (int i = 0; i < buffer_length; i++) {
    uint8_t c = 0x00;
    for (int j = 0; j < 8; j++) {
      c = c << 1;
      if(bool_buffer[index]){
        c = c | mask;
      }
      index++;
    }
    buffer[i] = c;
  }
  printf("\n");

  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      if(temp_bool_buffer[xyToIndex(x, y)]){
        printf("1 ");
      } else {
        printf("0 ");
      }
    }
    printf("\n");
  }


 
  ret = write(fd, buffer, buffer_length);

  printf("%d", buffer_length);

  if (ret < 0) {
    perror("Failed to write the message to the device.");
    return errno;
  }

  close(fd);

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
