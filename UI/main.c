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
#define CONTRAST_MAX 255

int screen_width = 0;
int screen_height = 0;
int buffer_length = 0;
int contrast = 200;

int xyToIndex(int x, int y, int w) { return x + (y * w); }

int map(int x, int in_min, int in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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
  ioctl(device_file, SET_CONTRAST, &contrast);

  buffer_length = (screen_width * screen_height) / 8;

  uint8_t buffer[buffer_length];
  bool pix_map[screen_width * screen_height];

  for (int i = 0; i < buffer_length; i++) {
    buffer[i] = 0x00;
  }

  int ret = write(device_file, buffer, buffer_length);

  for (int i = 0; i < screen_width * screen_height; i++) {
    pix_map[i] = false;
  }

  InitWindow(screen_width * PIXEL_SIZE + (10 * PIXEL_SIZE),
             screen_height * PIXEL_SIZE, "OLED Draw");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    mousePosition = GetMousePosition();
    BeginDrawing();

    ClearBackground(BLACK);

    DrawRectangle(screen_width * PIXEL_SIZE, 0, PIXEL_SIZE,
                  screen_height * PIXEL_SIZE, RED);

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
    DrawRectangle(
      (screen_width + 1) * PIXEL_SIZE, 
      (screen_height * PIXEL_SIZE) - map(contrast, 0, CONTRAST_MAX, 0, screen_height * PIXEL_SIZE), 
      9 * PIXEL_SIZE,
      (map(contrast, 0, CONTRAST_MAX, 0, screen_height) + 1) * PIXEL_SIZE, 
      WHITE
  );

    EndDrawing();

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      if (mousePosition.x < screen_width * PIXEL_SIZE) {
        int mousex = floor(mousePosition.x / PIXEL_SIZE);
        int mousey = floor(mousePosition.y / PIXEL_SIZE);

        if (!pix_map[xyToIndex(mousex, mousey, screen_width)]) {
          pix_map[xyToIndex(mousex, mousey, screen_width)] = true;
          pixelData.x = mousex;
          pixelData.y = mousey;
          ioctl(device_file, SET_PIXEL, &pixelData);
        }
      } else {
        contrast = map(mousePosition.y, screen_height * PIXEL_SIZE, 0, 0, CONTRAST_MAX);
        ioctl(device_file, SET_CONTRAST, &contrast);
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
