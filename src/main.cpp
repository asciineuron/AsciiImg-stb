#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "MyApp.hpp"
#include <QApplication>

int overlap() { return 0; }

long int strided_overlap(unsigned char *image, int swath_width,
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

char font_buffer[1 << 25];

void dofunc() {
  //char* font_buffer = (char*)malloc((1<<25)*sizeof(char));
  //std::vector<char> font_buffer;
  //font_buffer.reserve(1<<25);
  std::string input;
  while (std::getline(std::cin, input)) {

    int text_pixel_height = 10; // 50;
    text_pixel_height = std::stoi(input);
    // overview:
    // load image
    // load each and every character we want for printing
    FILE *font_file = fopen("../DejaVuSansMono.ttf", "r");
    fread(font_buffer, sizeof(char), 1 << 25, font_file);

    stbtt_fontinfo font;
    stbtt_InitFont(
        &font, (unsigned char *)font_buffer,
        stbtt_GetFontOffsetForIndex((unsigned char *)font_buffer, 0));

    // num AZaz09 = 62?
    int tot_chars = 62;
    int char_width, char_height = 0;
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
      // TODO note just to prefill sizes, take smallest or largest and do again
      // with stbtt_MakeCodepointBitmap
      // for (int i = 0; i < 1; i++) {
      // all_chars[i] = stbtt_GetCodepointBitmap(&font, 0,
      // stbtt_ScaleForMappingEmToPixels(&font, text_pixel_height),
      //     (int)current_char, &char_width, &char_height, 0, 0);
      all_chars_in[i] =
          stbtt_GetCodepointBitmap(&font, scale, scale, current_char,
                                   &char_widths[i], &char_heights[i], 0, 0);

      // printf("width: %d, height %d\n", char_width, char_height);
      //  ^ yup I confirmed the size of each is the same, however not same as
      //  text_pixel_height above
      //  TODO nope they may vary!!!
      char bmpname[100];
      snprintf(bmpname, 100, "./testout-%d.bmp", i);
      // stbi_write_bmp(bmpname, char_width, char_height, 1, all_chars[i]);

      if (current_char == 'z') {
        current_char = 'A';
      } else if (current_char == 'Z') {
        current_char = '0';
      } else {
        current_char += 1;
      }
    }

    for (int f = 0; f < tot_chars; f++) {
      // printf("width: %d, height %d\n", char_widths[f], char_heights[f]);
      if (char_widths[f] > char_width) {
        char_width = char_widths[f];
      }
      if (char_heights[f] > char_height) {
        char_height = char_heights[f];
      }
    }

    current_char = 'a';
    for (int i = 0; i < tot_chars; i++) {
      // for (int i = 0; i < 1; i++) {
      // all_chars[i] = stbtt_GetCodepointBitmap(&font, 0,
      // stbtt_ScaleForMappingEmToPixels(&font, text_pixel_height),
      //     (int)current_char, &char_width, &char_height, 0, 0);
      all_chars_res[i] =
          (unsigned char *)malloc(char_width * char_height * sizeof(char));
      stbtt_MakeCodepointBitmap(&font, all_chars_res[i], char_width,
                                char_height, char_width, scale, scale,
                                current_char);

      // printf("width: %d, height %d\n", char_width, char_height);
      //  ^ yup I confirmed the size of each is the same, however not same as
      //  text_pixel_height above
      //  TODO nope they may vary!!!
      char bmpname[100];
      snprintf(bmpname, 100, "./rescaled-testout-%d.bmp", i);
      stbi_write_bmp(bmpname, char_width, char_height, 1, all_chars_res[i]);

      if (current_char == 'z') {
        current_char = 'A';
      } else if (current_char == 'Z') {
        current_char = '0';
      } else {
        current_char += 1;
      }
    }

    int ii_x, ii_y, ii_n;
    unsigned char *input_image =
        stbi_load("../scuba.jpg", &ii_x, &ii_y, &ii_n, 1);
    // stbi_write_jpg("./testoutscuba.jpg", ii_x, ii_y, 1, input_image, 80);

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
          // printf("i %d j %d overlap: %ld\n", i, j, overlap);
          if (overlap > best_overlap) {
            best_char_idx = char_idx;
            best_overlap = overlap;
          }
        }
        // printf("best overlap idx %d and amt: %d\n", best_char_idx,
        // best_overlap);

        // now write best character to output image
        int out_offset_start = i * char_height * output_x + j * char_width;
        strided_copy(output_image, char_width, char_height, ii_x,
                     out_offset_start, all_chars_res[best_char_idx]);
      }
    }

    stbi_write_bmp("./convertedimage.bmp", output_x, output_y, 1, output_image);
    // for (int i = 0; i < ii_x*ii_y; i += char_width*char_height) {
    //   // extract sub block array from input image corresponding to current
    //   character chunk
    //   // get striding once based on image size
    //   for (int j = 0; j < tot_chars; j++) {
    //     // long overlap = strided_overlap(input_image, all_chars[i], ii_x,
    //     ii_y, char_width, char_height);
    //     // if overlap > max, set max index and overlap
    //     //
    //   }
    //   // replace/override copy image to be that best character
    // }
    //  compute x,y size of a (and since monospace, every) character
    //  step over image in chunks of that size
    //  loop each character, compute overlap, pick character with max overlap,
    //  print to buffer e.g SDL if leftover less than character x_sz, go to next
    //  row, continue until done
    printf("done\n");
  }
}

// since monospace can assume all fonts have same upper bounding box?
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MyApp mainapp;
  mainapp.show();
  return app.exec();
}
