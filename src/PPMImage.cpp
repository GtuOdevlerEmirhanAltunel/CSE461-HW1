#include "PPMImage.hpp"

#include <fstream>

PPMImage::PPMImage(int width, int height) : width(width), height(height) {
  Vec3s = new Vec3[width * height];
}

PPMImage PPMImage::operator=(const PPMImage &other) {
  if (this != &other) {
    delete[] Vec3s;
    width = other.width;
    height = other.height;
    Vec3s = new Vec3[width * height];
    for (int i = 0; i < width * height; ++i) {
      Vec3s[i] = other.Vec3s[i];
    }
  }
  return *this;
}

PPMImage::PPMImage(const PPMImage &other)
    : width(other.width), height(other.height) {
  Vec3s = new Vec3[width * height];
  for (int i = 0; i < width * height; ++i) {
    Vec3s[i] = other.Vec3s[i];
  }
}

PPMImage::~PPMImage() {
  delete[] Vec3s;
}

void PPMImage::setVec3(int x, int y, Vec3 Vec3) {
  int index = (y * width + x);
  Vec3s[index] = Vec3;
}

void PPMImage::save(const std::string &filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Could not open file for writing");
  }
  file << "P6\n" << width << " " << height << "\n255\n";
  for (int i = 0; i < width * height; ++i) {
    file << static_cast<unsigned char>(Vec3s[i].x)
         << static_cast<unsigned char>(Vec3s[i].y)
         << static_cast<unsigned char>(Vec3s[i].z);
  }
  file.close();
}
