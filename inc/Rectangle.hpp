#ifndef INC_RECTANGLE
#define INC_RECTANGLE

#include "SceneObject.hpp"
#include "Triangle.hpp"

class Rectangle : public SceneObject {
 public:
  Rectangle(const Vec3 &position, const Vec3 &direction1,
            const Vec3 &direction2, const Vec3 &color);
  bool intersect(const Ray &ray, float &t, Vec3 &color,
                 Vec3 &surfaceNormal) const override;

 private:
  Triangle triangle1;
  Triangle triangle2;
  Vec3 color;
};

#endif /* INC_RECTANGLE */
