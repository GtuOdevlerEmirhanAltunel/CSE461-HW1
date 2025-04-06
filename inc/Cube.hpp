#ifndef INC_CUBE
#define INC_CUBE

#include <array>

#include "Rectangle.hpp"
#include "SceneObject.hpp"

class Cube : public SceneObject {
 public:
  Cube(const Vec3 &position, const Vec3 &direction1, const Vec3 &direction2,
       const Vec3 &direction3, const Pixel &color);
  bool intersect(const Ray &ray, float &t, Pixel &color,
                 Vec3 &surfaceNormal) const override;

 protected:
  std::array<Rectangle, 6> faces;
};

#endif /* INC_CUBE */
