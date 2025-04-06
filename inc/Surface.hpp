#ifndef INC_SURFACE
#define INC_SURFACE

#include "SceneObject.hpp"

class Surface : public SceneObject {
 public:
  Surface(const Vec3 &position, const Vec3 &normal, const Pixel &color);

  bool intersect(const Ray &ray, float &t, Pixel &color,
                 Vec3 &surfaceNormal) const override;

 protected:
  Vec3 position;
  Vec3 normal;
  Pixel color;
};

#endif /* INC_SURFACE */
