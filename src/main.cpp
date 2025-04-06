#include "AmbientLight.hpp"
#include "Cube.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Surface.hpp"

int main() {
  Scene scene;
  Camera camera({0, 0, 3}, {0, 0.2, -1}, 90.0f, 800, 600);

  scene.addObject(
      std::make_shared<Surface>(Vec3{0, 3, 0}, Vec3{0, 1, 0}, WHITE));
  // scene.addObject(std::make_shared<Sphere>(Vec3{2, 2, -7}, 0.5f, WHITE));
  // scene.addObject(std::make_shared<Sphere>(Vec3{1, -5, -7}, 0.5f, YELLOW));
  scene.addObject(std::make_shared<Cube>(
      Vec3{1, 2, -7}, Vec3{1, 0, 0}, Vec3{0, 0.5, 0}, Vec3{0, 0, 0.5}, YELLOW));
  scene.addObject(std::make_shared<Cube>(
      Vec3{1, 2, -9}, Vec3{1, 0, 0}, Vec3{0, 0.5, 0}, Vec3{0, 0, 0.5}, WHITE));

  // scene.addLight(std::make_shared<PointLight>(Vec3{2, 0, -5}, BLUE, 0.6f));
  // scene.addLight(std::make_shared<PointLight>(Vec3{-2, 0, -5}, RED, 0.6f));
  scene.addLight(
      std::make_shared<DirectionalLight>(Vec3{0, 1, 0}, WHITE, 0.5f));
  scene.addLight(std::make_shared<AmbientLight>(WHITE, 0.2f));

  scene.render(camera).save("output.ppm");
  return 0;
}
