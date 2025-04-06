#ifndef INC_PPMIMAGE
#define INC_PPMIMAGE

#include <string>

#include "Pixel.hpp"

struct PPMImage {
  PPMImage(int width, int height);
  PPMImage operator=(const PPMImage &other);
  PPMImage(const PPMImage &other);

  ~PPMImage();

  void setPixel(int x, int y, Pixel pixel);

  void save(const std::string &filename);

  int width;
  int height;

  Pixel *pixels;
};

#endif /* INC_PPMIMAGE */
