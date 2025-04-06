#ifndef INC_POINTLIGHT
#define INC_POINTLIGHT

#include "Light.hpp"

class PointLight : public Light {
 public:
  PointLight(const Vec3 &position, const Pixel &color, float intensity)
      : Light(Light::Type::POINT),
        position(position),
        color(color),
        intensity(intensity) {
  }

  Vec3 position;
  Pixel color;
  float intensity;
};

#endif /* INC_POINTLIGHT */
