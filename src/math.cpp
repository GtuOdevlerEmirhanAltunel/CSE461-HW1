#include "math.hpp"

#include <cmath>

Number length(const Vec3& v) {
  return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 normalize(const Vec3& v) {
  Number length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  return Vec3(v.x / length, v.y / length, v.z / length);
}

Vec3 cross(const Vec3& a, const Vec3& b) {
  return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x);
}

Number dot(const Vec3& a, const Vec3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
  return v - n * (2.0f * dot(v, n));
}
