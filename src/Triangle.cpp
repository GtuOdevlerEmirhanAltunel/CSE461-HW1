#include "Triangle.hpp"

#include <iostream>

Triangle::Triangle(Vec3 v0, Vec3 v1, Vec3 v2, const Vec3 &color)
    : Surface(v0, (v0 - v1).cross(v2 - v1), color), v0(v0), v1(v1), v2(v2) {
}

static bool sameSide(const Vec3 &p1, const Vec3 &p2, const Vec3 &a,
                     const Vec3 &b) {
  Vec3 cp1 = (b - a).cross(p1 - a);
  Vec3 cp2 = (b - a).cross(p2 - a);
  return cp1.dot(cp2) >= 0;
}

bool Triangle::intersect(const Ray &ray, float &t, Vec3 &color,
                         Vec3 &surfaceNormal) const {
  Vec3 surfaceNormal_temp;
  float t_temp;
  if (Surface::intersect(ray, t_temp, color, surfaceNormal_temp)) {
    Vec3 point = ray.origin + ray.direction * t_temp;
    if (sameSide(point, v0, v1, v2) && sameSide(point, v1, v0, v2) &&
        sameSide(point, v2, v0, v1)) {
      t = t_temp;
      surfaceNormal = surfaceNormal_temp;
      return true;
    }
  }
  return false;
}
