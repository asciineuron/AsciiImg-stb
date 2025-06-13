#pragma once

#include "stb_truetype.h"
#include <QImage>
#include <string>
#include <vector>

namespace MyLib {

class AsciiImg {
public:
  AsciiImg(int pixsz, const std::string &font_name,
           const std::string &image_name);

  // void setPixSize(int size);
  void setTextRows(int num) { m_requestedSize = num; };
  void setPadFactor(float pad) { m_padFactor = pad; }; // only allow positive?
  void getImageData();
  bool loadImage(const std::string &image_name);

  void asciifyImage();
  const QImage& getAsciifyImage();
  const QImage& getOrigImage();

private:
  static constexpr int m_totChars = 60;
  QImage m_imData;
  QImage m_asciifiedImData;
  std::array<char, 1 << 25> m_fontBuffer{};
  stbtt_fontinfo m_fontInfo;
  std::vector<std::vector<unsigned char>> m_charsResized{};
  unsigned char *all_chars_in[m_totChars];
  int m_maxCharWidth = 0;
  int m_maxCharHeight = 0;
  // int m_pixSize = 0;
  int m_textRows = 10; // instead of pixsize, divide image into X rows. slider
                       // goes up to 1 down to something small
  int m_requestedSize = 0;
  int char_widths[m_totChars];
  int char_heights[m_totChars];
  bool m_hasAsciified = false;

  void loadFont(const std::string &filename);
  void rescaleFont();
  int im_x = 0;
  int im_y = 0;
  float m_padFactor = 1.0f; // scale of m_maxCharWidth/Height to give more space around letters
};

unsigned char *dofunc(const unsigned char *input_image, int *out_width,
                      int *out_height, int ii_x, int ii_y);

} // namespace MyLib
