#ifndef INC_POINTLIGHT
#define INC_POINTLIGHT

#include "Light.hpp"

class PointLight : public Light {
 public:
  PointLight(const Vec3 &position, const Vec3 &intensity)
      : Light(Light::Type::POINT), position(position), intensity(intensity) {
  }

  Vec3 position;
  Vec3 intensity;
};

#endif /* INC_POINTLIGHT */
