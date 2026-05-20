#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <raylib.h>

#define MSF_GIF_IMPL
#include "./thirdparty/msf_gif.h"

#define FLAG_IMPLEMENTATION
#include "./thirdparty/flag.h"

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
#define LED_CHAR_WIDTH 8

#define GIF_RECORD_FRAMERATE 5

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

      for (int k = 0; k < LED_CHAR_WIDTH; ++k) {
        board[row][(i * (LED_CHAR_WIDTH + 1) + k)] = (byte >> k) & 1;
      }
    }
  }
}

int board_move_left()
{
  static size_t scroll_offset = 0;

  return scroll_offset += 1;
}

void usage(FILE *stream)
{
    fprintf(stream, "Usage: ./ledsb [OPTIONS] [--] [ARGS]\n");
    fprintf(stream, "OPTIONS:\n");
    flag_print_options(stream);
}

int main(int argc, char **argv)
{
  bool *render_gif = flag_bool("render-gif", false, "Render gif out of screen recording");
  bool *help = flag_bool("help", false, "Print help");
  char **text = flag_str("text", "  EMPTY  ", "Text to render");

  if (!flag_parse(argc, argv)) {
    usage(stderr);
    flag_print_error(stderr);
    exit(1);
  }
  argc = flag_rest_argc();
  argv = flag_rest_argv();

  if (*help) {
    usage(stdout);
    exit(0);
  }

  int board[ROW_SIZE][SCROLL_COL_SIZE];
  memset(board, 0, sizeof board);

  size_t text_len = strlen(*text);
  board_write(board, *text);
  assert(text_len < SCROLL_COL_SIZE && "can not render text longer than SCROLL_COL_SIZE");

  size_t content_width = text_len * LED_CHAR_WIDTH + 1;
  size_t gap_between_loop = 5 * LED_CHAR_WIDTH;

  InitWindow(WIDTH, HEIGHT, "LED sign board");
  SetTargetFPS(20);

  MsfGifState gifState = {0};
  msf_gif_begin(&gifState, GetRenderWidth(), GetRenderHeight());
  int gif_frame_counter = 0;

  while(!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(GRAY);
    int scroll_offset = board_move_left();
    board_draw(board, scroll_offset, content_width + gap_between_loop);

    EndDrawing();

    if (*render_gif) {
      gif_frame_counter++;
      if (gif_frame_counter > GIF_RECORD_FRAMERATE) {
        Image im_screen = LoadImageFromScreen();

        msf_gif_frame(&gifState, im_screen.data, (int)((1.0f/60.0f)*GIF_RECORD_FRAMERATE)/15, 16, im_screen.width*4);
        UnloadImage(im_screen);

        gif_frame_counter = 0;
      }
    }
  }

  if (*render_gif) {
    MsfGifResult result = msf_gif_end(&gifState);
    if (result.data) {
      SaveFileData("screenrecording.gif", result.data, (unsigned int)result.dataSize);
    }
    msf_gif_free(result);
  }

  CloseWindow();

  return 0;
}
