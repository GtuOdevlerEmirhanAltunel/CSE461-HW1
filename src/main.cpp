#include "AmbientLight.hpp"
#include "Cube.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Surface.hpp"

int main() {
  Scene scene;

  // scene.addObject(
  //     std::make_shared<Surface>(Vec3{-3, -3, -3}, Vec3{0, -2, 0}, GREEN));
  // scene.addObject(std::make_shared<Sphere>(Vec3{0, 0, -7}, 0.5f, WHITE));
  // scene.addObject(std::make_shared<Sphere>(Vec3{1, 5, -7}, 0.5f, YELLOW));
  // scene.addObject(std::make_shared<Sphere>(Vec3{0.5, 5, -12}, 0.5f, YELLOW));
  // scene.addObject(std::make_shared<Cube>(
  //     Vec3{0, 4, -7}, Vec3{1, 0, 0}, Vec3{0, -0.5, 0}, Vec3{0, 0, -5},
  //     YELLOW));

  scene.readFromFile(
      "/home/demir/Documents/Projects/CSE461-HW1/assets/scene.xml");
  scene.render().save("output.ppm");
  return 0;
}
