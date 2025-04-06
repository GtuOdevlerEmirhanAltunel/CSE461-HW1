#ifndef INC_DIRECTIONALLIGHT
#define INC_DIRECTIONALLIGHT

#include "Light.hpp"

class DirectionalLight : public Light {
 public:
  DirectionalLight(const Vec3 &direction, const Pixel &color, float intensity)
      : Light(Light::Type::DIRECTIONAL),
        direction(direction),
        color(color),
        intensity(intensity) {
  }

  Vec3 direction;
  Pixel color;
  float intensity;
};

#endif /* INC_DIRECTIONALLIGHT */
