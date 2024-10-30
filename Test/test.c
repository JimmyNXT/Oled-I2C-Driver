#include "../Driver/SH1106_IOCTL.h"
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <unistd.h>

int xyToIndex(int x, int y, int w) { return x + (y * w); }

int main() {
  int ret, fd, width, height, buffer_length;
  fd = open("/dev/SH1106_DISPLAY",
            O_RDWR); // Open the device with read/write access

  if (fd < 0) {
    perror("Failed to open the device...");
    return errno;
  }
  // ioctl(fd, GET_WIDTH, &width);
  // ioctl(fd, GET_HEIGHT, &height);
  //
  // buffer_length = width * height / 8;
  //
  // uint8_t buffer[buffer_length];
  //
  // for (int i = 0; i < buffer_length; i++) {
  //   buffer[i] = 0xFF;
  // }
  //
  // ret = write(fd, buffer, buffer_length);



   // printf("%d", buffer_length);
  int contrast = 10;

  ioctl(fd, SET_CONTRAST, &contrast);

  if (ret < 0) {
    perror("Failed to write the message to the device.");
    return errno;
  }

  close(fd);

  return 0;
}
