#include "Vector.hpp"

#include <cmath>

Vec3 Vec3::operator+(const Vec3 &other) const {
  return {x + other.x, y + other.y, z + other.z};
}

Vec3 Vec3::operator-(const Vec3 &other) const {
  return {x - other.x, y - other.y, z - other.z};
}

float Vec3::dot(const Vec3 &other) const {
  return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::operator*(float scalar) const {
  return {x * scalar, y * scalar, z * scalar};
}

Vec3 Vec3::operator/(float scalar) const {
  return {x / scalar, y / scalar, z / scalar};
}

Vec3 Vec3::cross(const Vec3 &other) const {
  return {y * other.z - z * other.y, z * other.x - x * other.z,
          x * other.y - y * other.x};
}

Vec3 Vec3::normalize() const {
  float length = sqrt(x * x + y * y + z * z);
  return {x / length, y / length, z / length};
}

Vec3 Vec3::operator-() const {
  return {-x, -y, -z};
}

float Vec3::length() const {
  return sqrt(x * x + y * y + z * z);
}
