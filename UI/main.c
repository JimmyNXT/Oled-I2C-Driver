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
  int index = 0;
  uint8_t buffer[buffer_length];
  uint8_t mask = 0x01;
  uint8_t c = 0x00;

  if (device_file < 0) {
    printf("Failed to open the device file...");
    return errno;
  }

  ioctl(device_file, GET_WIDTH, &screen_width);
  printf("Width: %d\n", screen_width);
  ioctl(device_file, GET_HEIGHT, &screen_height);
  printf("Height: %d\n", screen_height);

  buffer_length = (screen_width * screen_height) / 8;

  bool pix_map[screen_width][screen_height];

  Vector2 mousePosition;
  int prevMouseX = -1;
  int prevMouseY = -1;
  for (int x = 0; x < screen_width; x++) {
    for (int y = 0; y < screen_height; y++) {
      pix_map[x][y] = false;
    }
  }

  for (int i = 0; i < screen_width; i++) {
    for (int j = 0; j < 8; j = j + 2) {
      pix_map[i][j] = true;
    }
  }
  InitWindow(screen_width * PIXEL_SIZE, screen_height * PIXEL_SIZE,
             "OLED Draw");

  SetTargetFPS(60);

  int frame_count = 0;
  while (!WindowShouldClose()) {
    mousePosition = GetMousePosition();
    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (int x = 0; x < screen_width; x++) {
      for (int y = 0; y < screen_height; y++) {
        Color c;
        if (pix_map[x][y]) {
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

      pix_map[mousex][mousey] = true;

      prevMouseX = mousex;
      prevMouseY = mousey;
    }
    if (frame_count % 10 == 0) {
      frame_count = 0;

      index = 0;
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < screen_width; j++) {
          c = 0x00;
          for (int k = 0; k < 8; k++) {
            c = c << 1;
            if (pix_map[j][i + k]) {
              c = c | mask;
            }
          }
          buffer[index] = c;
          index++;
        }
      }

      // for (int i = 0; i < buffer_length; i++) {
      //   uint8_t c = 0x00;
      //
      //   for (int j = 0; j < 8; j++) {
      //     c = c << 1;
      //     if(pix_map[index]){
      //       c = c | mask;
      //     }
      //     index = index + 1;
      //   }
      //   buffer[i] = c;
      // }
      int ret = write(device_file, buffer, buffer_length);
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      for (int x = 0; x < screen_width; x++) {
        for (int y = 0; y < screen_height; y++) {
          pix_map[x][y] = false;
        }
      }
    }
    frame_count++;
  }

  close(device_file);

  CloseWindow();
  return 0;
}
