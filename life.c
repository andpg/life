#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <math.h>

void CreateLife(bool squares[], int gridWidth, int gridHeight) {
  bool newSquares[gridWidth*gridHeight];
  int sumNeighbors;
  for (int i=0; i<gridHeight; i++) {
    for (int j=0; j<gridWidth; j++) {
      sumNeighbors = 0;
      if (j>0 && squares[i*gridWidth+(j-1)]) sumNeighbors++;
      if (j<gridWidth-1 && squares[i*gridWidth+(j+1)]) sumNeighbors++;
      if (i>0) {
        if (j>0 && squares[(i-1)*gridWidth+(j-1)]) sumNeighbors++;
        if (squares[(i-1)*gridWidth+j]) sumNeighbors++;
        if (j<gridWidth-1 && squares[(i-1)*gridWidth+(j+1)]) sumNeighbors++;
      }
      if (i<gridHeight-1) {
        if (j>0 && squares[(i+1)*gridWidth+(j-1)]) sumNeighbors++;
        if (squares[(i+1)*gridWidth+j]) sumNeighbors++;
        if (j<gridWidth-1 && squares[(i+1)*gridWidth+(j+1)]) sumNeighbors++;
      }
      if (squares[i*gridWidth+j]) {
        newSquares[i*gridWidth+j] = sumNeighbors == 2 || sumNeighbors == 3;
      } else {
        newSquares[i*gridWidth+j] = sumNeighbors == 3;
      }
    }
  }
  for (int i=0; i<gridWidth*gridHeight; i++)
    squares[i] = newSquares[i];
}

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

  int speed = 4;
  bool speedEdit = false;
  bool running = false;
  double startTime;
  int counter;

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

    if (GuiButton((Rectangle){ 650, 25, 125, 40 }, "#115#Next step")) {
      CreateLife(squares, gridWidth, gridHeight);
    }

    if(GuiSpinner((Rectangle){ 650, 345, 125, 30 }, NULL, &speed, 1, 6, speedEdit)) {
      speedEdit = !speedEdit;
      if (running) {
        counter = 0;
        startTime = GetTime();
      }
    }

    if (running) {
      if (((int)((GetTime()-startTime)*speed)) >= counter) {
        CreateLife(squares, gridWidth, gridHeight);
        counter++;
      }
      running = !GuiButton((Rectangle){ 650, 385, 125, 40 }, "#132#Pause");
    } else {
      if (GuiButton((Rectangle){ 650, 385, 125, 40 }, "#131#Start")) {
        running = true;
        counter = 0;
        startTime = GetTime();
      }
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
