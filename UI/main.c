#include <raylib.h>
#include <math.h>
#include <stdbool.h>
#include "../Driver/SH1106_IOCTL.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define PIXEL_SIZE 5

int screen_width = 0;
int screen_height = 0;
int buffer_length = 0;


int xyToIndex(int x, int y) { return x + (y * screen_width); }

int main(void) {
  int device_file = open("/dev/SH1106_DISPLAY", O_RDWR);

  if (device_file < 0) {
    printf("Failed to open the device file...");
    return errno;
  }

  ioctl(device_file, GET_WIDTH, &screen_width);
  printf("Width: %d\n", screen_width);
  ioctl(device_file, GET_HEIGHT, &screen_height);
  printf("Height: %d\n", screen_height);

  buffer_length = (screen_width * screen_height) / 8;

  bool pix_map[screen_width * screen_height];

  Vector2 mousePosition;
  int prevMouseX = -1;
  int prevMouseY = -1;
  for (int i = 0; i < (screen_width * screen_height); i++) {
    pix_map[i] = false;
  }
  InitWindow(screen_width * PIXEL_SIZE, screen_height * PIXEL_SIZE, "OLED Draw");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    mousePosition = GetMousePosition();
    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (int i = 0; i < screen_width; i += 1) {
      for (int j = 0; j < screen_height; j += 1) {
        Color c;
        if (pix_map[xyToIndex(i, j)]) {
          c = WHITE;
        } else {
          c = BLACK;
        }
        DrawRectangle(i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE,
                      c);
      }
    }

    EndDrawing();

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      int mousex = floor(mousePosition.x / PIXEL_SIZE);
      int mousey = floor(mousePosition.y / PIXEL_SIZE);

      pix_map[xyToIndex(mousex, mousey)] = true;

      prevMouseX = mousex;
      prevMouseY = mousey;
    }

    int index = 0;
    bool temp_pix_map[screen_width * screen_height];
    uint8_t buffer[buffer_length];
    uint8_t mask = 0x01;

    // for (int i = 0; i < (screen_height * screen_width); i++) {
    //   temp_pix_map[i] = false;
    // }


    for(int i = 0; i < screen_width; i = i + 8){
      for(int j = 0; j < screen_height; j = j + 8) {
        for(int x = i; x < i + 8; x++){
          for(int y = j; y < j + 8; y++){
            temp_pix_map[xyToIndex(x, y)] = pix_map[xyToIndex(y, x)];
          }
        }
      }
    }

    for (int i = 0; i < buffer_length; i = i + 1) {
      uint8_t c = 0x00;

      for (int j = 0; j < 8; j++) {
        if(temp_pix_map[index]){
          c = c | mask;
        }
        index = index + 1;
        c = c << 1;
      }
      buffer[i] = c; 
    }
    int ret = write(device_file, buffer, buffer_length);

    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
      for (int i = 0; i < screen_width * screen_width; i++) {
        pix_map[i] = false;
      }
    }
  }

  close(device_file);

  CloseWindow();
  return 0;
}
