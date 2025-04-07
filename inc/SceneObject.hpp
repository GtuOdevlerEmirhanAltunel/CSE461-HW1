#ifndef INC_SCENEOBJECT
#define INC_SCENEOBJECT

#include "Ray.hpp"
#include "Vector.hpp"

class SceneObject {
 public:
  virtual bool intersect(const Ray &ray, float &t, Vec3 &color,
                         Vec3 &surfaceNormal) const = 0;
};

#endif /* INC_SCENEOBJECT */
