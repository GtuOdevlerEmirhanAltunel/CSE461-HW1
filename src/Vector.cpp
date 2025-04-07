#include "Vector.hpp"

Vec3 Vec3::operator+(Vec3 other) const {
  return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator+=(Vec3 other) {
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}

Vec3 Vec3::operator-(Vec3 other) const {
  return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(Vec3 other) const {
  return Vec3(x * other.x, y * other.y, z * other.z);
}

Vec3 Vec3::operator*=(Vec3 other) {
  x *= other.x;
  y *= other.y;
  z *= other.z;
  return *this;
}

float Vec3::dot(Vec3 other) const {
  return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::operator*(float scalar) const {
  return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator/(float scalar) const {
  return Vec3(x / scalar, y / scalar, z / scalar);
}

Vec3 Vec3::cross(Vec3 other) const {
  return Vec3(y * other.z - z * other.y, z * other.x - x * other.z,
              x * other.y - y * other.x);
}

Vec3 Vec3::normalize() const {
  float length = sqrt(x * x + y * y + z * z);
  return Vec3(x / length, y / length, z / length);
}

Vec3 Vec3::operator-() const {
  return Vec3(-x, -y, -z);
}

float Vec3::length() const {
  return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::parseVec3(const std::string &str) {
  Vec3 v;
  std::stringstream ss(str);
  ss >> v.x >> v.y >> v.z;
  return v;
}
