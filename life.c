#include "raylib.h"

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(400,300,"Hello World");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Hello, world!", 20, 20, 20, BLACK);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
