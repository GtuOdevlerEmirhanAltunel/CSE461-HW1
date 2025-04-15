#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>

#include "scene.hpp"
#include "stb_image_write.h"

int main() {
  Scene scene(
      "/home/demir/Documents/Projects/CSE461-HW1/assets/scene copy.xml");
  Image image = scene.render();
  std::string outputFileName = "output.png";
  if (stbi_write_png(outputFileName.c_str(), image.width, image.height, 3,
                     image.data.data(), image.width * 3) == 0) {
    std::cerr << "Error writing image to file: " << outputFileName << std::endl;
    return 1;
  }
}
