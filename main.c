#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <raylib.h>

#include "./bitmap.c"

#define WIND_FAC 10
#define WIDTH 80*WIND_FAC
#define HEIGHT 8*WIND_FAC

#define PAD 2
#define BOARD_W 8
#define BOARD_H 8

#define ROW_SIZE HEIGHT/BOARD_H
#define COL_SIZE WIDTH/BOARD_W
#define SCROLL_COL_SIZE 4096
#define CHAR_WIDTH 8

#define ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))

void board_draw(int board[ROW_SIZE][SCROLL_COL_SIZE], size_t scroll_offset, size_t content_width)
{
  for (size_t i = 0; i < ROW_SIZE; ++i) {
    for (size_t j = 0; j < COL_SIZE; ++j) {
      DrawRectangleV(
          (Vector2) { .x = j * (BOARD_W + PAD), .y = i * (BOARD_H + PAD) },
          (Vector2) { .x = BOARD_W,  .y = BOARD_H },
          board[i][(j + scroll_offset) % content_width] >= 1 ? SKYBLUE : DARKGRAY
      );
    }
  }
}

void board_write(int board[ROW_SIZE][SCROLL_COL_SIZE], char text[])
{
  for (size_t i = 0; i < strlen(text); ++i) {
    int code_point = (int)text[i];
    size_t row_len = ARRAY_LEN(font8x8_basic[code_point]);

    for (size_t row = 0; row < row_len; ++row) {
      unsigned char byte = font8x8_basic[code_point][row];

      for (int k = 0; k < CHAR_WIDTH; ++k) {
        board[row][(i * (CHAR_WIDTH + 1) + k)] = (byte >> k) & 1;
      }
    }
  }
}

int board_move_left(size_t content_width)
{
  static size_t scroll_offset = 0;

  return scroll_offset += 1;
}

int main()
{
  int board[ROW_SIZE][SCROLL_COL_SIZE];
  memset(board, 0, sizeof board);

  char text[] = "Mahdi <3 Narges";
  size_t text_len = strlen(text);
  board_write(board, text);
  assert(text_len < SCROLL_COL_SIZE && "can not render text longer than SCROLL_COL_SIZE");

  size_t content_width = text_len * CHAR_WIDTH + 1;
  size_t gap_between_loop = 5 * CHAR_WIDTH;

  InitWindow(WIDTH, HEIGHT, "LED sign board");
  SetTargetFPS(20);

  while(!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(GRAY);
    int scroll_offset = board_move_left(content_width);
    board_draw(board, scroll_offset, content_width + gap_between_loop);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
