#ifndef INC_LIGHT
#define INC_LIGHT

#include "SceneObject.hpp"

class Light {
 public:
  enum class Type { POINT, AMBIENT, DIRECTIONAL };

  Light(Type type) : type(type) {
  }
  virtual ~Light() = default;
  Type getType() const {
    return type;
  }

 protected:
  Type type;
};

#endif /* INC_LIGHT */
