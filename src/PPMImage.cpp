#include "PPMImage.hpp"

#include <fstream>

PPMImage::PPMImage(int width, int height) : width(width), height(height) {
  pixels = new Pixel[width * height];
}

PPMImage PPMImage::operator=(const PPMImage &other) {
  if (this != &other) {
    delete[] pixels;
    width = other.width;
    height = other.height;
    pixels = new Pixel[width * height];
    for (int i = 0; i < width * height; ++i) {
      pixels[i] = other.pixels[i];
    }
  }
  return *this;
}

PPMImage::PPMImage(const PPMImage &other)
    : width(other.width), height(other.height) {
  pixels = new Pixel[width * height];
  for (int i = 0; i < width * height; ++i) {
    pixels[i] = other.pixels[i];
  }
}

PPMImage::~PPMImage() {
  delete[] pixels;
}

void PPMImage::setPixel(int x, int y, Pixel pixel) {
  int index = (y * width + x);
  pixels[index] = pixel;
}

void PPMImage::save(const std::string &filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Could not open file for writing");
  }
  file << "P6\n" << width << " " << height << "\n255\n";
  file.write(reinterpret_cast<char *>(pixels), width * height * sizeof(Pixel));
  file.close();
}
