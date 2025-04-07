#include "Cube.hpp"

#include <limits>

Cube::Cube(const Vec3 &position, const Vec3 &direction1, const Vec3 &direction2,
           const Vec3 &direction3, const Vec3 &color)
    : faces({Rectangle(position, direction1, direction2, color),
             Rectangle(position, direction1, direction3, color),
             Rectangle(position, direction2, direction3, color),
             Rectangle(position + direction1, direction2, direction3, color),
             Rectangle(position + direction2, direction1, direction3, color),
             Rectangle(position + direction3, direction1, direction2, color)}) {
}

bool Cube::intersect(const Ray &ray, float &t, Vec3 &color,
                     Vec3 &surfaceNormal) const {
  bool hit = false;
  float closest_t = std::numeric_limits<float>::max();
  Vec3 closest_normal;
  for (int i = 0; i < 6; ++i) {
    float t_temp;
    Vec3 normal_temp;
    if (faces[i].intersect(ray, t_temp, color, normal_temp) &&
        t_temp < closest_t) {
      closest_t = t_temp;
      closest_normal = normal_temp;
      hit = true;
    }
  }
  if (hit) {
    t = closest_t;
    surfaceNormal = closest_normal;
  }
  return hit;
}
