#ifndef MYLIB_H
#define MYLIB_H

#include "stb_truetype.h"
#include <string>
#include <vector>

namespace MyLib {

class AsciiImg {
public:
  AsciiImg(std::string& filename);

  void setPixSize(int size);

private:
  unsigned char* m_imData = nullptr;
  char* m_fontBuffer = nullptr; 
  //int m_textBuffer = nullptr;
  stbtt_fontinfo m_fontInfo; 
  //unsigned char** m_charsResized;
  std::vector<unsigned char*> m_charsResized;
  int m_maxCharWidth;
  int m_maxCharHeight;
  int m_imDataX;
  int m_imDataY; 
  int m_pixSize;
};

unsigned char* dofunc(const unsigned char* input_image, int* out_width, int* out_height, int ii_x, int ii_y);

unsigned char* loadimage(std::string_view filename);
}

#endif
