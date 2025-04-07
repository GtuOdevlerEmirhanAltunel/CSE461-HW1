#ifndef INC_SPHERE
#define INC_SPHERE

#include "SceneObject.hpp"

class Sphere : public SceneObject {
 public:
  Sphere(const Vec3 &center, float radius, const Vec3 &color);
  bool intersect(const Ray &ray, float &t, Vec3 &color,
                 Vec3 &surfaceNormal) const override;

 private:
  Vec3 center;
  float radius;
  Vec3 color;
};

#endif /* INC_SPHERE */
