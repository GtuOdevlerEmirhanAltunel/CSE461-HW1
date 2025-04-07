#ifndef INC_AMBIENTLIGHT
#define INC_AMBIENTLIGHT

#include "Light.hpp"

class AmbientLight : public Light {
 public:
  AmbientLight(Vec3 intensity)
      : Light(Light::Type::AMBIENT), intensity(intensity) {
  }

  Vec3 intensity;
};

#endif /* INC_AMBIENTLIGHT */
