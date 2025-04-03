#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_truetype.h"
#include <cstdio>
#include <cstdlib>

int overlap() { return 0; }

char font_buffer[1 << 25];

// since monospace can assume all fonts have same upper bounding box?
int main(int argc, char* argv[])
{
    int text_pixel_height = 50;
    // overview:
    // load image
    // load each and every character we want for printing
    FILE* font_file = fopen("../../DejaVuSansMono.ttf", "r");
    fread(font_buffer, sizeof(char), 1 << 25, font_file);

    stbtt_fontinfo font;
    stbtt_InitFont(&font, (unsigned char*)font_buffer,
        stbtt_GetFontOffsetForIndex((unsigned char*)font_buffer, 0));

    // num AZaz09 = 62?
    int tot_chars = 62;
    int char_width, char_height;
    // unsigned char *all_chars[size_t(('z' - 'a' + 1) + ('Z' - 'A' + 1) + ('9' - '0' + 1))];
    unsigned char* all_chars[size_t(('z' - 'a' + 1) + ('Z' - 'A' + 1) + ('9' - '0' + 1))];
    char current_char = 'a';
    for (int i = 0; i < tot_chars; i++) {
    //for (int i = 0; i < 1; i++) {
        //all_chars[i] = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForMappingEmToPixels(&font, text_pixel_height),
        //    (int)current_char, &char_width, &char_height, 0, 0);
         all_chars[i] = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, text_pixel_height),
             current_char, &char_width, &char_height, 0, 0);

        // printf("width: %d, height %d\n", char_width, char_height);
        // ^ yup I confirmed the size of each is the same, however not same as text_pixel_height above
        // char bmpname[100];
        // snprintf(bmpname, 100, "./testout-%d.bmp", i);
        // stbi_write_bmp(bmpname, char_width, char_height, 1, all_chars[i]);

        if (current_char == 'z') {
            current_char = 'A';
        } else if (current_char == 'Z') {
            current_char = '0';
        } else {
          current_char += 1;
        }
    }

    int ii_x, ii_y, ii_n;
    unsigned char* input_image = stbi_load("../../scuba.jpg", &ii_x, &ii_y, &ii_n, 1);
    stbi_write_jpg("./testoutscuba.bmp", ii_x, ii_y, 1, input_image, 80);

    // step in size of text chunk over image and find best match
    // this isnt quite right:
    for (int i = 0; i < ii_x*ii_y, i += char_width*char_height) {
      // extract sub block array from input image corresponding to current character chunk
      // get striding once based on image size
      for (jnt j = 0; j < tot_chars; j++) {
        // long overlap = strided_overlap(input_image, all_chars[i], ii_x, ii_y, char_width, char_height);
        // if overlap > max, set max index and overlap
        //
      }
      // replace/override copy image to be that best character
    }
    // compute x,y size of a (and since monospace, every) character
    // step over image in chunks of that size
    // loop each character, compute overlap, pick character with max overlap, print to buffer e.g SDL
    // if leftover less than character x_sz, go to next row, continue until done
}
