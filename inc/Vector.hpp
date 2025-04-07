#ifndef INC_VECTOR
#define INC_VECTOR

#include <cmath>
#include <sstream>
#include <string>
struct Vec3 {
  float x = 0;
  float y = 0;
  float z = 0;

  constexpr Vec3() = default;
  constexpr Vec3(float x, float y, float z) : x(x), y(y), z(z) {
  }

  Vec3 operator+(Vec3 other) const;
  Vec3 operator+=(Vec3 other);
  Vec3 operator-(Vec3 other) const;
  Vec3 operator*(Vec3 other) const;
  Vec3 operator*=(Vec3 other);
  float dot(Vec3 other) const;
  Vec3 operator*(float scalar) const;
  Vec3 operator/(float scalar) const;
  Vec3 cross(Vec3 other) const;
  Vec3 normalize() const;
  Vec3 operator-() const;
  float length() const;
  static Vec3 parseVec3(const std::string &str);
};

constexpr Vec3 BLACK(0, 0, 0);
constexpr Vec3 WHITE(255, 255, 255);
constexpr Vec3 RED(255, 0, 0);
constexpr Vec3 GREEN(0, 255, 0);
constexpr Vec3 BLUE(0, 0, 255);
constexpr Vec3 YELLOW(255, 255, 0);
constexpr Vec3 CYAN(0, 255, 255);
constexpr Vec3 MAGENTA(255, 0, 255);

#endif /* INC_VECTOR */
