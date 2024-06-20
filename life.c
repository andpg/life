#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "style_bluish.h"
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
  const int screenWidth = 650;
  const int screenHeight = 500;
  const int recWidth = 600;
  const int recHeight = 400;

  enum Size {
    _20x30,
    _24x16,
    _15x10,
    _12x8
  };
  int gridSize = 0;
  int squareSize = 0;
  int gridWidth = 0;
  int gridHeight = 0;

  int numSquares = 20*30; // Maximum grid size
  bool squares[numSquares];
  for (int i=0; i<numSquares; i++) squares[i] = false;

  InitWindow(screenWidth, screenHeight, "Game of Life");
  GuiLoadStyleBluish();
  SetTargetFPS(60);

  Rectangle gameRec = { 25, 25, recWidth, recHeight };

  int currentGesture = GESTURE_NONE;
  Vector2 touchPosition = { 0, 0 };

  int speed = 4;
  bool speedEdit = false;
  bool running = false;
  double startTime = 0;
  int counter = 0;

  while (!WindowShouldClose()) {
    currentGesture = GetGestureDetected();
    touchPosition = GetTouchPosition(0);

    switch (gridSize) {
      case _20x30:
        squareSize = 20; gridWidth = 30; gridHeight = 20; break;
      case _24x16:
        squareSize = 25; gridWidth = 24; gridHeight = 16; break;
      case _15x10:
        squareSize = 40; gridWidth = 15; gridHeight = 10; break;
      case _12x8:
        squareSize = 50; gridWidth = 12; gridHeight = 8; break;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangleLinesEx(gameRec, 2.0f, BLACK);
    for (int i = 1; i < gridWidth; i++) {
      DrawLine(25+squareSize*i, 25, 25+squareSize*i, 25+recHeight, BLACK);
    }
    for (int i = 1; i < gridHeight; i++) {
      DrawLine(25, 25+squareSize*i, 25+recWidth, 25+squareSize*i, BLACK);
    }
    for (int i = 0; i < gridWidth*gridHeight; i++) {
      if (squares[i])
        DrawRectangleV((Vector2){ 25+squareSize*(i%gridWidth), 25+squareSize*(i/gridWidth) }, (Vector2){ squareSize, squareSize }, BLACK);
    }

    if (CheckCollisionPointRec(touchPosition, gameRec) && currentGesture == GESTURE_TAP) {
      int tappedSquare = floorf((touchPosition.x - 25)/squareSize) + floorf((touchPosition.y - 25)/squareSize)*gridWidth;
      squares[tappedSquare] = !squares[tappedSquare];
    }

    GuiComboBox((Rectangle){ 25, 440, 150, 35 }, "30x20;24x16;15x10;12x8", &gridSize);

    if (GuiButton((Rectangle){ 295, 440, 100, 35 }, "#115#Next step")) {
      CreateLife(squares, gridWidth, gridHeight);
    }

    if(GuiSpinner((Rectangle){ 410, 440, 100, 35 }, NULL, &speed, 1, 6, speedEdit)) {
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
      running = !GuiButton((Rectangle){ 525, 440, 100, 35 }, "#132#Pause");
    } else {
      if (GuiButton((Rectangle){ 525, 440, 100, 35 }, "#131#Start")) {
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
