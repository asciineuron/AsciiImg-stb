#include "mylib.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace MyLib {
long int strided_overlap(const unsigned char *image, int swath_width,
                         int swath_height, int stride, int offset_start,
                         unsigned char *letter) {
  // assume n_channels = 1 so both monochromatic, each byte 0-255 greyscale
  long overlap = 0;
  int letter_idx = 0;
  for (int row = 0; row < swath_height; row++) {
    for (int col = 0; col < swath_width; col++) {
      char im_pix = image[offset_start + row * stride + col];
      char let_pix = letter[letter_idx];
      overlap += ((int)im_pix) * ((int)let_pix);

      letter_idx++;
    }
  }
  return overlap;
}

void strided_copy(unsigned char *output, int swath_width, int swath_height,
                  int stride, int offset_start, unsigned char *letter) {
  int letter_idx = 0;
  for (int row = 0; row < swath_height; row++) {
    for (int col = 0; col < swath_width; col++) {
      output[offset_start + row * stride + col] = letter[letter_idx];

      letter_idx++;
    }
  }
}

//static char font_buffer[1 << 25];

unsigned char* dofunc(const unsigned char* input_image, int* out_width, int* out_height, int ii_x, int ii_y) {
  char *font_buffer = new char[1<<25];
  int text_pixel_height = 20; // 50;
  //text_pixel_height =std::stoi(input); TODO get input here

  FILE *font_file = fopen("/Volumes/Ext/Code/AsciiImg-stb/DejaVuSansMono.ttf", "r");
  if (!font_file) {
    printf("cant open font\n");
    return nullptr;
  }
  printf("read file\n");
  fread(font_buffer, sizeof(char), 1 << 25, font_file);
  fclose(font_file);

  stbtt_fontinfo font;
  stbtt_InitFont(
      &font, (unsigned char *)font_buffer,
      stbtt_GetFontOffsetForIndex((unsigned char *)font_buffer, 0));

  // num AZaz09 = 62?
  int tot_chars = 62;
  int char_width = 0;
  int char_height = 0;
  int char_widths[62];
  int char_heights[62];
  // unsigned char *all_chars[size_t(('z' - 'a' + 1) + ('Z' - 'A' + 1) + ('9'
  // - '0' + 1))];
  unsigned char *all_chars_in[size_t(('z' - 'a' + 1) + ('Z' - 'A' + 1) +
      ('9' - '0' + 1))];
  unsigned char *all_chars_res[size_t(('z' - 'a' + 1) + ('Z' - 'A' + 1) +
('9' - '0' + 1))];
  char current_char = 'a';

  float scale = stbtt_ScaleForPixelHeight(&font, text_pixel_height);
  scale = stbtt_ScaleForMappingEmToPixels(&font, text_pixel_height);

  for (int i = 0; i < tot_chars; i++) {
    all_chars_in[i] =
      stbtt_GetCodepointBitmap(&font, scale, scale, current_char,
          &char_widths[i], &char_heights[i], 0, 0);

    if (current_char == 'z') {
      current_char = 'A';
    } else if (current_char == 'Z') {
      current_char = '0';
    } else {
      current_char += 1;
    }
  }

  for (int f = 0; f < tot_chars; f++) {
    if (char_widths[f] > char_width) {
      char_width = char_widths[f];
    }
    if (char_heights[f] > char_height) {
      char_height = char_heights[f];
    }
  }

  current_char = 'a';
  for (int i = 0; i < tot_chars; i++) {
    all_chars_res[i] =
      (unsigned char *)malloc(char_width * char_height * sizeof(char));

    stbtt_MakeCodepointBitmap(&font, all_chars_res[i], char_width,
        char_height, char_width, scale, scale,
        current_char);

    if (current_char == 'z') {
      current_char = 'A';
    } else if (current_char == 'Z') {
      current_char = '0';
    } else {
      current_char += 1;
    }
  }

  //int ii_x, ii_y, ii_n;
  //unsigned char *input_image =
  //  stbi_load("../scuba.jpg", &ii_x, &ii_y, &ii_n, 1);

  // step in size of text chunk over image and find best match
  // this isnt quite right:
  // exclude the trailing parts of image...
  int ascii_per_row = ii_x / char_width;
  int ascii_per_col = ii_y / char_height;

  int output_x = ascii_per_row * char_width;
  int output_y = ascii_per_col * char_height;
  unsigned char *output_image =
    (unsigned char *)malloc(output_x * output_y * sizeof(char));
  memset(output_image, 0,
      (ascii_per_row * ascii_per_col) * (char_width * char_height));

  for (int i = 0; i < ascii_per_col; i++) {
    for (int j = 0; j < ascii_per_row; j++) {
      int best_char_idx = 0;
      long best_overlap = 0;
      int offset_start = i * char_height * ii_x + j * char_width;

      for (int char_idx = 0; char_idx < tot_chars; char_idx++) {
        long overlap =
          strided_overlap(input_image, char_width, char_height, ii_x,
              offset_start, all_chars_res[char_idx]);
        if (overlap > best_overlap) {
          best_char_idx = char_idx;
          best_overlap = overlap;
        }
      }
      // printf("best overlap idx %d and amt: %d\n", best_char_idx,
      // best_overlap);

      // now write best character to output image
      int out_offset_start = i * char_height * output_x + j * char_width;
      //strided_copy(input_image, char_width, char_height, ii_x,
          //out_offset_start, all_chars_res[best_char_idx]);
      strided_copy(output_image, char_width, char_height, ii_x,
          out_offset_start, all_chars_res[best_char_idx]);
    }
  }
  delete[] font_buffer;
  *out_width = output_x;
  *out_height = output_y;
  return output_image;
}


unsigned char* loadimage(std::string_view filename) {
  return nullptr;
}
}
