#ifndef MYLIB_H
#define MYLIB_H

#include "stb_truetype.h"
#include <QImage>
#include <string>
#include <vector>

namespace MyLib {

class AsciiImg {
public:
  AsciiImg(int pixsz, const std::string &font_name,
           const std::string &image_name);

  //void setPixSize(int size);
  void setTextRows(int num);
  void getImageData();
  bool loadImage(const std::string &image_name);

  QImage asciifyImage();

private:
  static constexpr int m_totChars = 60;
  QImage m_imData;
  std::array<char, 1 << 25> m_fontBuffer{};
  stbtt_fontinfo m_fontInfo;
  std::vector<std::vector<unsigned char>> m_charsResized{};
  int m_maxCharWidth = 0;
  int m_maxCharHeight = 0;
  //int m_pixSize = 0;
  int m_textRows = 10; // instead of pixsize, divide image into X rows. slider goes up to 1 down to something small
  int m_requestedSize = 0;
  int char_widths[m_totChars];
  int char_heights[m_totChars];

  void loadFont(const std::string &filename);
  void rescaleFont();
};

unsigned char *dofunc(const unsigned char *input_image, int *out_width,
                      int *out_height, int ii_x, int ii_y);

} // namespace MyLib

#endif
