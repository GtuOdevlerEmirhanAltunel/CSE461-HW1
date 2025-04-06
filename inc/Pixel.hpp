#ifndef INC_PIXEL
#define INC_PIXEL

struct Pixel {
  unsigned char r;
  unsigned char g;
  unsigned char b;

  Pixel operator+(const Pixel &other) const;
  Pixel operator+=(const Pixel &other);
  Pixel operator*(const Pixel &other) const;
  Pixel operator*=(const Pixel &other);
  Pixel operator*(float scalar) const;
};

constexpr Pixel BLACK{0, 0, 0};
constexpr Pixel WHITE{255, 255, 255};
constexpr Pixel RED{255, 0, 0};
constexpr Pixel GREEN{0, 255, 0};
constexpr Pixel BLUE{0, 0, 255};
constexpr Pixel YELLOW{255, 255, 0};
constexpr Pixel CYAN{0, 255, 255};
constexpr Pixel MAGENTA{255, 0, 255};

#endif /* INC_PIXEL */
