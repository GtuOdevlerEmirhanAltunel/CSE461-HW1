#ifndef INC_CAMERA
#define INC_CAMERA

#include "Vector.hpp"

struct Camera {
  Camera(const Vec3 &position, const Vec3 &direction, float fov, int width,
         int height)
      : position(position),
        direction(direction.normalize()),
        fov(fov),
        width(width),
        height(height) {
  }

  Vec3 position;
  Vec3 direction;
  float fov;
  int width;
  int height;
};

#endif /* INC_CAMERA */
