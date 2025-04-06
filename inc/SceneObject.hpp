#ifndef INC_SCENEOBJECT
#define INC_SCENEOBJECT

#include "Pixel.hpp"
#include "Ray.hpp"

class SceneObject {
 public:
  virtual bool intersect(const Ray &ray, float &t, Pixel &color,
                         Vec3 &surfaceNormal) const = 0;
};

#endif /* INC_SCENEOBJECT */
