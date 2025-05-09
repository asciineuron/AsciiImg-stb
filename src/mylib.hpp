#ifndef MYLIB_H
#define MYLIB_H

#include <string_view>

namespace MyLib {

unsigned char* dofunc(const unsigned char* input_image, int* out_width, int* out_height, int ii_x, int ii_y);

unsigned char* loadimage(std::string_view filename);
}

#endif
