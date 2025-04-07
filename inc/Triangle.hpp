#ifndef INC_TRIANGLE
#define INC_TRIANGLE

#include "SceneObject.hpp"
#include "Surface.hpp"

class Triangle : public Surface {
 public:
  Triangle(Vec3 v0, Vec3 v1, Vec3 v2, const Vec3 &color);
  bool intersect(const Ray &ray, float &t, Vec3 &color,
                 Vec3 &surfaceNormal) const override;

 protected:
  Vec3 v0, v1, v2;
};

#endif /* INC_TRIANGLE */
