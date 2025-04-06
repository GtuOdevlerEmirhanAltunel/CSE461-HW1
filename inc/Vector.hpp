#ifndef INC_VECTOR
#define INC_VECTOR

struct Vec3 {
  float x;
  float y;
  float z;

  Vec3 operator+(const Vec3 &other) const;
  Vec3 operator-(const Vec3 &other) const;
  float dot(const Vec3 &other) const;
  Vec3 operator*(float scalar) const;
  Vec3 operator/(float scalar) const;
  Vec3 cross(const Vec3 &other) const;
  Vec3 normalize() const;
  Vec3 operator-() const;
  float length() const;
};

#endif /* INC_VECTOR */
