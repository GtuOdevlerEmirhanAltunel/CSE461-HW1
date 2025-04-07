#ifndef INC_CAMERA
#define INC_CAMERA

#include "Vector.hpp"

struct Camera {
  Camera() = default;

  Vec3 position;
  Vec3 direction;
  Vec3 up;
  float left, right, bottom, top;
  float nearDistance;
  int width, height;
};

#endif /* INC_CAMERA */
