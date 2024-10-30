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
  ioctl(fd, GET_WIDTH, &width);
  ioctl(fd, GET_HEIGHT, &height);

  buffer_length = width * height / 8;
  //
  uint8_t buffer[1024] = {
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000110, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000,
      0b11100001, 0b00000001, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000000, 0b00000100,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b10000000, 0b00011111, 0b00001000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b10000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000,
      0b01100000, 0b00001000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b01000000, 0b10000000, 0b00001000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00100000, 0b00001000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b01000000, 0b00100000, 0b11000100, 0b00001110, 0b00000000,
      0b00000000, 0b00000000, 0b00100000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00100000,
      0b00100000, 0b00111011, 0b00010000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00100000, 0b10100000, 0b00001111,
      0b00100000, 0b00111110, 0b00000000, 0b00000000, 0b00000000, 0b00000100,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b01110000, 0b00010111, 0b00010000, 0b00001111, 0b00100000, 0b11111100,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b10011000, 0b00010111, 0b00001000,
      0b11000100, 0b00001010, 0b10100000, 0b11110000, 0b00000000, 0b00000000,
      0b00000100, 0b00010000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b11100100, 0b00101111, 0b00100110, 0b11000100, 0b00010111,
      0b11010000, 0b11100000, 0b00000000, 0b00000000, 0b00000000, 0b00000100,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11100111,
      0b01011111, 0b00100000, 0b10000000, 0b00110011, 0b11010000, 0b11110000,
      0b00000100, 0b00000000, 0b00001000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b01100000, 0b11100000, 0b01111111, 0b00100000,
      0b00010000, 0b00010101, 0b01010000, 0b11111000, 0b00000110, 0b00000000,
      0b11001000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00011000, 0b00111100, 0b11111111, 0b01000110, 0b00000000, 0b00010011,
      0b01010000, 0b11111000, 0b00000111, 0b00000000, 0b01100100, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000011,
      0b11111111, 0b01111111, 0b00000000, 0b00000011, 0b01110000, 0b11111000,
      0b00000011, 0b00000000, 0b00100100, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00001000, 0b10000001, 0b11111111, 0b11111111,
      0b11110001, 0b00001100, 0b11110000, 0b01100111, 0b00011101, 0b00000000,
      0b00000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b11000110, 0b11111111, 0b11111111, 0b01011111, 0b00001000,
      0b00001010, 0b01001100, 0b11000000, 0b01111000, 0b00010001, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000100, 0b11001100,
      0b11111111, 0b11111111, 0b11111111, 0b00001010, 0b00001000, 0b11000000,
      0b00000000, 0b11111110, 0b00001000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000100, 0b11001000, 0b11111111, 0b01111111,
      0b11001110, 0b00001001, 0b00001101, 0b01100000, 0b00000010, 0b01110000,
      0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000100, 0b11110000, 0b11111111, 0b00111111, 0b00000100, 0b00001010,
      0b00000100, 0b10100000, 0b00000011, 0b00000000, 0b00000011, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b11110000,
      0b11111111, 0b00011111, 0b00011100, 0b00001110, 0b00000010, 0b00010000,
      0b00000010, 0b11111110, 0b00000011, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00001000, 0b11100100, 0b11111111, 0b00011111,
      0b00110000, 0b00001100, 0b00000001, 0b01001000, 0b00000010, 0b11111110,
      0b00000111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00001000, 0b11110000, 0b11111111, 0b00001111, 0b01010000, 0b10000000,
      0b00000000, 0b11101100, 0b00000111, 0b11111110, 0b00000111, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b11110000,
      0b11111111, 0b00001111, 0b11110000, 0b01000011, 0b00000000, 0b00110110,
      0b11111100, 0b11111111, 0b00000011, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00100000, 0b11110000, 0b11111111, 0b00000111,
      0b00000000, 0b11111111, 0b00000011, 0b00011001, 0b10000000, 0b11111111,
      0b00000011, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b01000000, 0b11110000, 0b11111111, 0b00000111, 0b00000000, 0b00011100,
      0b11111100, 0b00001100, 0b10000000, 0b11111111, 0b00000111, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b11110000,
      0b11111111, 0b00000111, 0b00000000, 0b11100000, 0b00000000, 0b00000110,
      0b11000000, 0b11111111, 0b00001111, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b10000000, 0b11110000, 0b11111111, 0b00000111,
      0b00000000, 0b00000000, 0b11000111, 0b00000001, 0b11100000, 0b11111111,
      0b00011111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b10000000, 0b11110000, 0b11111111, 0b00000111, 0b00000000, 0b00000000,
      0b00111000, 0b00000000, 0b11110000, 0b11111111, 0b00111111, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b11110000,
      0b11111111, 0b00000011, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b11111100, 0b11111111, 0b01111111, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b10000000, 0b11110000, 0b11111111, 0b00000011,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111110, 0b11111111,
      0b11111111, 0b00000001, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b10000000, 0b11110000, 0b11111111, 0b00000011, 0b00000000, 0b00000000,
      0b00000000, 0b10000000, 0b11111111, 0b11111111, 0b11111111, 0b00000011,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b11110000,
      0b11111111, 0b00000011, 0b00000000, 0b00000000, 0b00000000, 0b11000000,
      0b11111111, 0b11111111, 0b11111111, 0b00000111, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b11111110, 0b11111111, 0b00000011,
      0b00000000, 0b00000000, 0b00000000, 0b00110000, 0b11111111, 0b11111111,
      0b01111111, 0b00001111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b11110000, 0b11111111, 0b00000011, 0b00000000, 0b00000000,
      0b00000000, 0b11111000, 0b11111110, 0b11111111, 0b11011111, 0b00011001,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11110000,
      0b11111111, 0b00000011, 0b00000000, 0b00000000, 0b00000000, 0b11100100,
      0b11111101, 0b11111111, 0b11101111, 0b00110011, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b11110000, 0b11111111, 0b00000111,
      0b00000000, 0b00000000, 0b00000000, 0b11100010, 0b11111111, 0b11111111,
      0b11110111, 0b00100111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b11100000, 0b11111111, 0b00001111, 0b00000000, 0b00000000,
      0b00000000, 0b11100010, 0b11101111, 0b11111111, 0b11111101, 0b00100100,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11110000,
      0b11111111, 0b00000111, 0b00000000, 0b00000000, 0b00000000, 0b10100010,
      0b10111111, 0b11111111, 0b01111110, 0b00100100, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b11100000, 0b11111111, 0b00000111,
      0b00000000, 0b00000000, 0b00000000, 0b00100100, 0b11111111, 0b10011100,
      0b00011111, 0b00100100, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b11100000, 0b01111111, 0b00001000, 0b00000000, 0b00000000,
      0b00000000, 0b01100100, 0b11111100, 0b11111111, 0b00001111, 0b00010010,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01100000,
      0b00111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01001000,
      0b11111000, 0b11111111, 0b00000011, 0b00010010, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00100000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b01001000, 0b11100000, 0b11111111,
      0b00000000, 0b00001001, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00011000, 0b00000000, 0b00011000, 0b00000000, 0b00000000,
      0b00000000, 0b10010000, 0b00000000, 0b00001111, 0b10000000, 0b00000100,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000100,
      0b00000000, 0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00010000,
      0b00000001, 0b00000000, 0b11000000, 0b00000100, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b00000000, 0b01000000,
      0b00000000, 0b00000000, 0b00000000, 0b00100000, 0b00000001, 0b00000000,
      0b01000000, 0b00000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000001, 0b00000000, 0b10000000, 0b00000001, 0b00000000,
      0b00000000, 0b00100000, 0b00000010, 0b00000000, 0b00100000, 0b00000001,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00000000,
      0b00111000, 0b00000000, 0b00000001, 0b00000000, 0b00000000, 0b01000000,
      0b00000010, 0b00000000, 0b00011000, 0b00000001, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b01000000, 0b00000000, 0b01001000, 0b00000000,
      0b00000011, 0b00000000, 0b00000000, 0b11000000, 0b00001100, 0b00000000,
      0b10010100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b11000000, 0b00000000, 0b11000100, 0b00000000, 0b00000100, 0b00000000,
      0b00000000, 0b10000000, 0b00001011, 0b00000000, 0b01000100, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000000,
      0b00001000, 0b00000001, 0b00001000, 0b00000000, 0b00000000, 0b10000000,
      0b00001111, 0b00000000, 0b01110010, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b10000000, 0b00000000, 0b00010000, 0b00000001,
      0b00001000, 0b00000000, 0b00000000, 0b00000000, 0b00010010, 0b00000000,
      0b00010001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b10000000, 0b00000000, 0b00110000, 0b00000001, 0b10001000, 0b00000011,
      0b00000000, 0b00000000, 0b00100010, 0b10000000, 0b00001000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000000,
      0b01000000, 0b00000011, 0b01010000, 0b00000100, 0b00000000, 0b00001110,
      0b01100100, 0b01100000, 0b00001100, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000001, 0b00100000, 0b00000101,
      0b00110000, 0b00000110, 0b11000000, 0b11111001, 0b11001111, 0b00010000,
      0b01000010, 0b00011100, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b11110000, 0b00000001, 0b01000000, 0b00000011, 0b00110000, 0b00000110,
      0b11000000, 0b00110000, 0b10000000, 0b00011111, 0b00000000, 0b00110000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011100,
      0b11000000, 0b00000001, 0b00000000, 0b00000011, 0b11110000, 0b01111111,
      0b10000000, 0b11111111, 0b01111111, 0b00111110, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00001000, 0b00000000, 0b10000000, 0b00000001,
      0b10010000, 0b00000111, 0b11000000, 0b10111111, 0b11100000, 0b11111111,
      0b00101111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b11110000, 0b00000011, 0b11000000, 0b11111111, 0b11111111, 0b00000011,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111,
      0b00111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
      0b00000000, 0b00000000, 0b00000000, 0b00000000};
  //
  // for (int i = 0; i < buffer_length; i++) {
  //   buffer[i] = 0xFF;
  // }

  ret = write(fd, buffer, buffer_length);

  int ifile = open("./image.bin",O_RDWR);

  ret = write(ifile, buffer, buffer_length);

  close(ifile);

  // printf("%d", buffer_length);
  // int contrast = 300;
  //
  // ioctl(fd, GET_CONTRAST, &contrast);
  //
  // printf("%d\n", contrast);
  //
  // contrast = 200;
  //
  // ioctl(fd, SET_CONTRAST, &contrast);
  //
  // ioctl(fd, GET_CONTRAST, &contrast);
  //
  // printf("%d\n", contrast);
  //
  // read(fd, &buffer, buffer_length);
  //
  // for (int i = 0; i < buffer_length; i++) {
  //   printf("%d\n", buffer[i]);
  // }

  struct set_pixel pixelData;
  pixelData.colour = 0;

  // for (int i = 0; i < height; i++) {
  //   pixelData.x = i;
  //   pixelData.y = i;
  //   ioctl(fd, SET_PIXEL, &pixelData);
  // }
  //
  if (ret < 0) {
    perror("Failed to write the message to the device.");
    return errno;
  }

  close(fd);

  return 0;
}
