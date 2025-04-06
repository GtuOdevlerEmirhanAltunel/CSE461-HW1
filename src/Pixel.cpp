#include "Pixel.hpp"

#include <algorithm>

Pixel Pixel::operator+(const Pixel &other) const {
  Pixel result;
  result.r = std::min(r + other.r, 255);
  result.g = std::min(g + other.g, 255);
  result.b = std::min(b + other.b, 255);
  return result;
}

Pixel Pixel::operator+=(const Pixel &other) {
  r = std::min(r + other.r, 255);
  g = std::min(g + other.g, 255);
  b = std::min(b + other.b, 255);
  return *this;
}

Pixel Pixel::operator*(const Pixel &other) const {
  Pixel result;
  result.r = std::min(r * other.r / 255, 255);
  result.g = std::min(g * other.g / 255, 255);
  result.b = std::min(b * other.b / 255, 255);
  return result;
}

Pixel Pixel::operator*=(const Pixel &other) {
  r = std::min(r * other.r / 255, 255);
  g = std::min(g * other.g / 255, 255);
  b = std::min(b * other.b / 255, 255);
  return *this;
}

Pixel Pixel::operator*(float scalar) const {
  Pixel result;
  result.r = std::min(static_cast<int>(r * scalar), 255);
  result.g = std::min(static_cast<int>(g * scalar), 255);
  result.b = std::min(static_cast<int>(b * scalar), 255);
  return result;
}
