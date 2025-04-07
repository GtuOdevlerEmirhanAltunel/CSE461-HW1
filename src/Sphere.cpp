#include "Sphere.hpp"

#include <cmath>

Sphere::Sphere(const Vec3 &center, float radius, const Vec3 &color)
    : center(center), radius(radius), color(color) {
}

bool Sphere::intersect(const Ray &ray, float &t, Vec3 &color,
                       Vec3 &surfaceNormal) const {
  // d^2 * t^2 + 2 * d * oc * t + oc^2 - r^2 = 0
  Vec3 oc = ray.origin - center;
  float a = ray.direction.dot(ray.direction);
  float b = 2.0f * oc.dot(ray.direction);
  float c = oc.dot(oc) - radius * radius;
  float discriminant = b * b - 4 * a * c;
  if (discriminant > 0) {
    t = (-b - sqrt(discriminant)) / (2.0f * a);
    if (t >= 0) {
      color = this->color;
      surfaceNormal = (ray.origin + ray.direction * t - center).normalize();
      return true;
    }
  }
  return false;
}
