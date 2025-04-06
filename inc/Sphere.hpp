#ifndef INC_SPHERE
#define INC_SPHERE

#include "SceneObject.hpp"

class Sphere : public SceneObject {
 public:
  Sphere(const Vec3 &center, float radius, const Pixel &color);
  bool intersect(const Ray &ray, float &t, Pixel &color,
                 Vec3 &surfaceNormal) const override;

 private:
  Vec3 center;
  float radius;
  Pixel color;
};

#endif /* INC_SPHERE */
