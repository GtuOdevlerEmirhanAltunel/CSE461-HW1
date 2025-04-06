#ifndef INC_AMBIENTLIGHT
#define INC_AMBIENTLIGHT

#include "Light.hpp"

class AmbientLight : public Light {
 public:
  AmbientLight(const Pixel &color, float intensity)
      : Light(Light::Type::AMBIENT), color(color), intensity(intensity) {
  }

  Pixel color;
  float intensity;
};

#endif /* INC_AMBIENTLIGHT */
