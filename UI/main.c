#include "../Driver/SH1106_IOCTL.h"
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define PIXEL_SIZE 5

int screen_width = 0;
int screen_height = 0;
int buffer_length = 0;

int xyToIndex(int x, int y, int w) { return x + (y * w); }

int main(void) {
  int device_file = open("/dev/SH1106_DISPLAY", O_RDWR);
  uint8_t mask = 0x01;
  struct set_pixel pixelData;
  pixelData.colour = 1;
  Vector2 mousePosition;

  if (device_file < 0) {
    printf("Failed to open the device file...");
    return errno;
  }

  ioctl(device_file, GET_WIDTH, &screen_width);
  printf("Width: %d\n", screen_width);
  ioctl(device_file, GET_HEIGHT, &screen_height);
  printf("Height: %d\n", screen_height);

  buffer_length = (screen_width * screen_height) / 8;

  uint8_t buffer[buffer_length];
  bool pix_map[screen_width * screen_height];

  read(device_file, &buffer, buffer_length);

  for (int x = 0; x < screen_width / 8; x++) {
    for (int y = 0; y < screen_height; y++) {
      uint8_t c = buffer[xyToIndex(x, y, screen_width / 8)];
      for (int xi = 7; xi >= 0; xi--) {
        uint8_t temp_c = c & mask;
        pix_map[xyToIndex(xi + (x * screen_width / 8), y, screen_width)] =
            temp_c == mask;
        c = c >> 1;
      }
    }
  }

  InitWindow(screen_width * PIXEL_SIZE, screen_height * PIXEL_SIZE,
             "OLED Draw");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    mousePosition = GetMousePosition();
    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (int x = 0; x < screen_width; x++) {
      for (int y = 0; y < screen_height; y++) {
        Color c;
        if (pix_map[xyToIndex(x, y, screen_width)]) {
          c = WHITE;
        } else {
          c = BLACK;
        }
        DrawRectangle(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE,
                      c);
      }
    }

    EndDrawing();

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      int mousex = floor(mousePosition.x / PIXEL_SIZE);
      int mousey = floor(mousePosition.y / PIXEL_SIZE);

      if (!pix_map[xyToIndex(mousex, mousey, screen_width)]) {
        pix_map[xyToIndex(mousex, mousey, screen_width)] = true;
        pixelData.x = mousex;
        pixelData.y = mousey;
        ioctl(device_file, SET_PIXEL, &pixelData);
      }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      for (int x = 0; x < screen_width; x++) {
        for (int y = 0; y < screen_height; y++) {
          pix_map[xyToIndex(x, y, screen_width)] = false;
        }
      }
      for (int i = 0; i < buffer_length; i++) {
        buffer[i] = 0x00;
      }

      int ret = write(device_file, buffer, buffer_length);
    }
  }

  close(device_file);

  CloseWindow();
  return 0;
}
