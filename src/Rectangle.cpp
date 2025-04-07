#include "Rectangle.hpp"

Rectangle::Rectangle(const Vec3 &position, const Vec3 &direction1,
                     const Vec3 &direction2, const Vec3 &color)
    : triangle1(position, position + direction1, position + direction2, color),
      triangle2(position + direction1, position + direction1 + direction2,
                position + direction2, color),
      color(color) {
}

bool Rectangle::intersect(const Ray &ray, float &t, Vec3 &color,
                          Vec3 &surfaceNormal) const {
  float t_temp;
  Vec3 surfaceNormal_temp;
  bool hit1 = triangle1.intersect(ray, t_temp, color, surfaceNormal_temp);
  if (hit1) {
    t = t_temp;
    surfaceNormal = surfaceNormal_temp;
    return true;
  }
  bool hit2 = triangle2.intersect(ray, t_temp, color, surfaceNormal_temp);
  if (hit2) {
    t = t_temp;
    surfaceNormal = surfaceNormal_temp;
    return true;
  }
  return false;
}
