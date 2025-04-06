#include "Surface.hpp"

Surface::Surface(const Vec3 &position, const Vec3 &normal, const Pixel &color)
    : position(position), normal(normal), color(color) {
}

bool Surface::intersect(const Ray &ray, float &t, Pixel &color,
                        Vec3 &surfaceNormal) const {
  // t = (a - o) . n / (d . n)
  float denom = normal.dot(ray.direction);
  if (denom > 1e-6 || denom < -1e-6) {
    Vec3 p0l0 = position - ray.origin;
    float t_temp = p0l0.dot(normal) / denom;
    if (t_temp >= 0) {
      t = t_temp;
      color = this->color;
      surfaceNormal = normal;
      return true;
    }
  }
  return false;
}
