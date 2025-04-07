#ifndef INC_PPMIMAGE
#define INC_PPMIMAGE

#include <string>

#include "Vector.hpp"

struct PPMImage {
  PPMImage(int width, int height);
  PPMImage operator=(const PPMImage &other);
  PPMImage(const PPMImage &other);

  ~PPMImage();

  void setVec3(int x, int y, Vec3 Vec3);

  void save(const std::string &filename);

  int width;
  int height;

  Vec3 *Vec3s;
};

#endif /* INC_PPMIMAGE */
