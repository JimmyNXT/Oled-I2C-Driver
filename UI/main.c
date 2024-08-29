#include "raylib.h"

#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define OLED_WIDTH 128
#define OLED_HEIGHT 64


bool pixMap[OLED_WIDTH * OLED_HEIGHT]

int main(void) {
  InitWindow(SCREEN_HEIGHT, SCREEN_HEIGHT,
             "raylib [core] example - basic window");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawRectangle(0, 0, GetScreenWidth() / 2, 30, Fade(RAYWHITE, 0.6f));

    // DrawText("Congrats! You created your first window!", 190, 200, 20,
    //          LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow(); // Close window and OpenGL context
  return 0;
}
