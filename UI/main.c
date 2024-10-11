#include <raylib.h>
#include <math.h>
#include <stdbool.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define PIXEL_SIZE 5

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

    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
      reset_Screen();
    }
  }

  CloseWindow();
  return 0;
}
