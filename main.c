#include <stdio.h>
#include <string.h>
#include <raylib.h>

#include "./bitmap.c"

#define WIND_FAC 10
#define WIDTH 80*WIND_FAC
#define HEIGHT 60*WIND_FAC

#define PAD 1
#define BOARD_W 9
#define BOARD_H 9

#define ROW_SIZE HEIGHT/BOARD_H
#define COL_SIZE WIDTH/BOARD_W

#define ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))

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
  int char_width = 8;

  for (size_t i = 0; i < strlen(text); ++i) {
    int code_point = (int)text[i];
    size_t row_len = ARRAY_LEN(font8x8_basic[code_point]);

    for (size_t row = 0; row < row_len; ++row) {
      unsigned char byte = font8x8_basic[code_point][row];

      for (int k = 0; k < char_width; ++k) {
        board[row][i * (char_width + 1) + k] = (byte >> k) & 1;
      }
    }
  }
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
