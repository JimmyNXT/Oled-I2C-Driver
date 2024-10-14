#include <raylib.h>
#include <math.h>
#include <stdbool.h>
#include "../Driver/SH1106.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#define SCREEN_WIDTH SH1106_LCDWIDTH
#define SCREEN_HEIGHT SH1106_LCDHEIGHT
#define PIXEL_SIZE 5
#define BUFFER_LENGTH SH1106_LCDWIDTH * SH1106_LCDHEIGHT / 8 

bool pix_map[SCREEN_WIDTH * SCREEN_HEIGHT];

void reset_Screen() {
  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
    pix_map[i] = false;
  }
}

int xyToIndex(int x, int y) { return x + (y * SCREEN_WIDTH); }

int main(void) {
  Vector2 mousePosition;
  int prevMouseX = -1;
  int prevMouseY = -1;

  reset_Screen();

  InitWindow(SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE, "OLED Draw");
  
  int fd;

  fd = open("/dev/SH1106_DISPLAY", O_RDWR); // Open the device with read/write access

  if (fd < 0) {
    perror("Failed to open the device...");
    return errno;
  }

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    mousePosition = GetMousePosition();
    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (int i = 0; i < SCREEN_WIDTH; i += 1) {
      for (int j = 0; j < SCREEN_HEIGHT; j += 1) {
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
    uint8_t buffer[BUFFER_LENGTH];
    uint8_t mask = 0x01;


    for (int i = 0; i < BUFFER_LENGTH; i = i + 1) {
      uint8_t c = 0x00;

      for (int j = 0; j < 8; j = j + 1) {
        if(pix_map[index]){
          c = c | mask;
        }
        index = index + 1;
        c = c << 1;
      }
      buffer[i] = c; 
    }
    int ret = write(fd, buffer, BUFFER_LENGTH);

    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
      reset_Screen();
    }
  }

  close(fd);

  CloseWindow();
  return 0;
}
