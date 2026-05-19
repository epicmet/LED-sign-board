#include <stdio.h>
#include <string.h>
#include <raylib.h>

#define WIND_FAC 10
#define WIDTH 80*WIND_FAC
#define HEIGHT 60*WIND_FAC

#define PAD 10
#define BOARD_W 90
#define BOARD_H 90

#define ROW_SIZE HEIGHT/BOARD_H
#define COL_SIZE WIDTH/BOARD_W


void draw_board(int board[ROW_SIZE][COL_SIZE])
{

  for (size_t i = 0; i < ROW_SIZE; ++i) {
    for (size_t j = 0; j < COL_SIZE; ++j) {
      DrawRectangleV(
          (Vector2) { .x = j * (BOARD_W + PAD), .y = i * (BOARD_H + PAD) },
          (Vector2) { .x = BOARD_W,  .y = BOARD_H},
          board[i][j] >= 1 ? SKYBLUE : DARKGRAY
      );
    }
  }
}

void board_write(int board[ROW_SIZE][COL_SIZE], char text[])
{
  board[0][0] = 1;
  board[1][0] = 1;
  board[2][0] = 1;
  board[3][0] = 1;

  board[0][1] = 1;

  board[1][2] = 1;
  board[2][3] = 1;

  board[1][4] = 1;
  board[0][5] = 1;

  board[0][6] = 1;
  board[1][6] = 1;
  board[2][6] = 1;
  board[3][6] = 1;

}

int main()
{
  int w = WIDTH;
  int h = HEIGHT;

  int board[ROW_SIZE][COL_SIZE];
  memset(board, 0, sizeof board);
  printf("board initialized with %d/%d dimensions\n", ROW_SIZE, COL_SIZE);

  board_write(board, "Mahdi");

  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(w, h, "LED sign board");
  SetTargetFPS(60);

  while(!WindowShouldClose()) {
    w = GetScreenWidth();
    h = GetScreenHeight();

    BeginDrawing();
    draw_board(board);

    ClearBackground(GRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
