#ifndef INC_DIRECTIONALLIGHT
#define INC_DIRECTIONALLIGHT

#include "Light.hpp"

class DirectionalLight : public Light {
 public:
  DirectionalLight(Vec3 direction, Vec3 intensity)
      : Light(Light::Type::DIRECTIONAL),
        direction(direction),
        intensity(intensity) {
  }

  Vec3 direction;
  Vec3 intensity;
};

#endif /* INC_DIRECTIONALLIGHT */
