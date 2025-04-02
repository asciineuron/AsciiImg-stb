#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <cstdio>
#include <cstdlib>

int overlap() { return 0; }

// since monospace can assume all fonts have same upper bounding box?
int main(int argc, char* argv[])
{
    int text_pixel_height = 10;
    // overview:
    // load image
    // load each and every character we want for printing
    char font_buffer[1 << 25];
    FILE* font_file = fopen("../DejaVuSansMono.ttf", "r");
    fread(font_buffer, sizeof(char), 1 << 25, font_file);

    stbtt_fontinfo font;
    stbtt_InitFont(&font, (unsigned char *)font_buffer, stbtt_GetFontOffsetForIndex((unsigned char *)font_buffer, 0));
    // num AZaz09 = 62?
    int tot_chars = 62;
    int char_width, char_height;
    // unsigned char *all_chars[size_t(('z' - 'a' + 1) + ('Z' - 'A' + 1) + ('9' - '0' + 1))];
    unsigned char* all_chars[size_t(('z' - 'a' + 1) + ('Z' - 'A' + 1) + ('9' - '0' + 1))];
    char current_char = 'a';
    for (int i = 0; i < tot_chars; i++) {
        all_chars[i] = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, text_pixel_height),
            current_char, &char_width, &char_height, 0, 0);

        if (current_char == 'z') {
            current_char = 'A';
        } else if (current_char == 'Z') {
            current_char = '0';
        }
    }

    // compute x,y size of a (and since monospace, every) character
    // step over image in chunks of that size
    // loop each character, compute overlap, pick character with max overlap, print to buffer e.g SDL
    // if leftover less than character x_sz, go to next row, continue until done
}
