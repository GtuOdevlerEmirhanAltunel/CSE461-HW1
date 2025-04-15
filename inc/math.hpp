#ifndef INC_MATH
#define INC_MATH

#include <cmath>
#include <cstdint>

using Number = float;
using Index = uint32_t;
constexpr Number EPSILON = 1e-4f;

struct Vec3 {
  Number x;
  Number y;
  Number z;

  Vec3() : x(0), y(0), z(0) {}
  Vec3(Number x, Number y, Number z) : x(x), y(y), z(z) {}
  Vec3 operator*(Number scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
  }
  Vec3 operator*(const Vec3& other) const {
    return Vec3(x * other.x, y * other.y, z * other.z);
  }
  Vec3 operator+(const Vec3& other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
  }
  Vec3 operator+(Number scalar) const {
    return Vec3(x + scalar, y + scalar, z + scalar);
  }
  Vec3 operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  Vec3 operator-(const Vec3& other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
  }
  Vec3 operator-() const { return Vec3(-x, -y, -z); }
  Vec3 operator/(Number scalar) const {
    return Vec3(x / scalar, y / scalar, z / scalar);
  }
  Vec3 operator^(Number exponent) const {
    return Vec3(std::pow(x, exponent), std::pow(y, exponent),
                std::pow(z, exponent));
  }
};

inline Vec3 operator-(Number scalar, const Vec3& v) {
  return Vec3(scalar - v.x, scalar - v.y, scalar - v.z);
}

struct Vec2 {
  Number u;
  Number v;

  Vec2() : u(0), v(0) {}
  Vec2(Number u, Number v) : u(u), v(v) {}
  Vec2 operator*(Number scalar) const { return Vec2(u * scalar, v * scalar); }
  Vec2 operator+(const Vec2& other) const {
    return Vec2(u + other.u, v + other.v);
  }
};

struct Ray {
  Vec3 origin;
  Vec3 direction;
};

Number length(const Vec3& v);
Vec3 normalize(const Vec3& v);
Vec3 cross(const Vec3& a, const Vec3& b);
Number dot(const Vec3& a, const Vec3& b);
Vec3 reflect(const Vec3& v, const Vec3& n);

#endif /* INC_MATH */
