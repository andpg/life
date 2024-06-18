#include "raylib.h"
#include <math.h>

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;
  const int recWidth = 600;
  const int recHeight = 400;
  const int gridSize = 20;
  int gridWidth = recWidth/gridSize;
  int gridHeight = recHeight/gridSize;

  int numSquares = gridHeight*gridWidth;
  bool squares[numSquares];
  for (int i=0; i<numSquares; i++) squares[i] = false;
  int tappedSquare = -1;

  InitWindow(screenWidth, screenHeight, "Game of Life");
  SetTargetFPS(60);

  Rectangle gameRec = { 25, 25, recWidth, recHeight };

  int currentGesture = GESTURE_NONE;
  Vector2 touchPosition = { 0, 0 };

  while (!WindowShouldClose()) {
    currentGesture = GetGestureDetected();
    touchPosition = GetTouchPosition(0);

    BeginDrawing();
    ClearBackground(DARKBLUE);
    DrawRectangleLinesEx(gameRec, 2.0f, RAYWHITE);
    for (int i = 1; i < gridWidth; i++) {
      DrawLine(25+gridSize*i, 25, 25+gridSize*i, 25+recHeight, RAYWHITE);
    }
    for (int i = 1; i < gridHeight; i++) {
      DrawLine(25, 25+gridSize*i, 25+recWidth, 25+gridSize*i, RAYWHITE);
    }
    for (int i = 0; i < numSquares; i++) {
      if (squares[i])
        DrawRectangleV((Vector2){ 25+gridSize*(i%gridWidth), 25+gridSize*(i/gridWidth) }, (Vector2){ gridSize, gridSize }, RAYWHITE);
    }

    if (CheckCollisionPointRec(touchPosition, gameRec) && currentGesture == GESTURE_TAP) {
      tappedSquare = floorf((touchPosition.x - 25)/gridSize) + floorf((touchPosition.y - 25)/gridSize)*gridWidth;
      squares[tappedSquare] = !squares[tappedSquare];
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
