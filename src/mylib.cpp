#include "mylib.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize2.h"
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
                         unsigned char *letter);
void strided_copy(unsigned char *output, int swath_width, int swath_height,
                  int stride, int offset_start, unsigned char *letter);

void AsciiImg::loadFont(const std::string &filename) {
  FILE *font_file = std::fopen(filename.c_str(), "r");
  if (!font_file) {
    printf("can't open file\n");
    return;
  }
  fread(m_fontBuffer.data(), sizeof(char), m_fontBuffer.size(), font_file);
  fclose(font_file);

  stbtt_InitFont(
      &m_fontInfo, (unsigned char *)m_fontBuffer.data(),
      stbtt_GetFontOffsetForIndex((unsigned char *)m_fontBuffer.data(), 0));
}

// void AsciiImg::setPixSize(int size) { m_requestedSize = size; }
void AsciiImg::setTextRows(int num) { m_requestedSize = num; }

void AsciiImg::rescaleFont() {
  // rescales based on m_pixSize
  unsigned char *all_chars_in[m_totChars];

  int pixsz = m_imData.size().height() / m_textRows;
  float scale = stbtt_ScaleForPixelHeight(&m_fontInfo, pixsz);
  // scale = stbtt_ScaleForMappingEmToPixels(&m_fontInfo, m_pixSize);

  char current_char = 'a';
  for (int i = 0; i < m_totChars; i++) {
    all_chars_in[i] =
        stbtt_GetCodepointBitmap(&m_fontInfo, scale, scale, current_char,
                                 &char_widths[i], &char_heights[i], 0, 0);

    if (current_char == 'z') {
      current_char = 'A';
    } else if (current_char == 'Z') {
      current_char = '0';
    } else {
      current_char += 1;
    }
  }

  // reset counting the max size for the new font size
  m_maxCharWidth = 0;
  m_maxCharHeight = 0;
  for (int f = 0; f < m_totChars; f++) {
    if (char_widths[f] > m_maxCharWidth) {
      m_maxCharWidth = char_widths[f];
    }
    if (char_heights[f] > m_maxCharHeight) {
      m_maxCharHeight = char_heights[f];
    }
  }

  current_char = 'a';
  for (int i = 0; i < m_totChars; i++) {
    m_charsResized[i].assign(m_maxCharWidth * m_maxCharHeight, 0);

    // stbtt_MakeCodepointBitmap(&m_fontInfo, m_charsResized[i].data(),
    //                           m_maxCharWidth, m_maxCharHeight,
    //                           m_maxCharWidth, scale, scale, current_char);
    stbtt_MakeCodepointBitmap(&m_fontInfo, m_charsResized[i].data(),
                              char_widths[i], char_heights[i], m_maxCharWidth,
                              scale, scale, current_char);

    if (current_char == 'z') {
      current_char = 'A';
    } else if (current_char == 'Z') {
      current_char = '0';
    } else {
      current_char += 1;
    }
  }
}

QImage AsciiImg::asciifyImage() {
  if (m_requestedSize != m_textRows) {
    m_textRows = m_requestedSize;
    rescaleFont();
  }
  int im_x = m_imData.size().width();
  int im_y = m_imData.size().height();

  int ascii_per_row = im_x / m_maxCharWidth;
  int ascii_per_col = im_y / m_maxCharHeight;

  int outszx = ascii_per_row * m_maxCharWidth;
  int outszy = ascii_per_col * m_maxCharHeight;

  QImage outImage(QSize(outszx, outszy), QImage::Format_Grayscale8);

  for (int i = 0; i < ascii_per_col; i++) {
    for (int j = 0; j < ascii_per_row; j++) {
      int best_char_idx = 0;
      long best_overlap = 0;
      int offset_start = i * m_maxCharHeight * im_x + j * m_maxCharWidth;

      for (int char_idx = 0; char_idx < m_totChars; char_idx++) {
        long overlap = strided_overlap(m_imData.bits(), m_maxCharWidth,
                                       m_maxCharHeight, im_x, offset_start,
                                       m_charsResized[char_idx].data());
        if (overlap > best_overlap) {
          best_char_idx = char_idx;
          best_overlap = overlap;
        }
      }
      // now write best character to output image
      int out_offset_start = i * m_maxCharHeight * outszx + j * m_maxCharWidth;
      //strided_copy(outImage.bits(), m_maxCharWidth, m_maxCharHeight, im_x,
      //             out_offset_start, m_charsResized[best_char_idx].data());
      strided_copy(outImage.bits(), char_widths[best_char_idx], char_heights[best_char_idx], im_x,
                   out_offset_start, m_charsResized[best_char_idx].data());
    }
  }
  return outImage;
}

bool AsciiImg::loadImage(const std::string &image_name) {
  QImage tryImage(image_name.c_str());
  if (tryImage.isNull()) {
    return false;
  }
  tryImage.convertTo(QImage::Format_Grayscale8);
  m_imData = tryImage;
  return true;
}

AsciiImg::AsciiImg(int pixsz, const std::string &font_name,
                   const std::string &image_name)
    : m_imData(image_name.c_str()), m_textRows(pixsz) {
  m_imData.convertTo(QImage::Format_Grayscale8);
  m_charsResized.insert(m_charsResized.begin(), m_totChars,
                        std::vector<unsigned char>());
  loadFont(font_name); // populates m_charsResized and m_fontBuffer
  rescaleFont();
}

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

} // namespace MyLib
