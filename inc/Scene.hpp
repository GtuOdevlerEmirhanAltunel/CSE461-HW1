#ifndef INC_SCENE
#define INC_SCENE

#include <memory>
#include <vector>

#include "Camera.hpp"
#include "Light.hpp"
#include "PPMImage.hpp"
#include "SceneObject.hpp"

class Scene {
 public:
  Scene() = default;
  void addObject(std::shared_ptr<SceneObject> object);
  void addLight(std::shared_ptr<Light> light);
  PPMImage render() const;
  bool occluded(const Ray &ray, float t) const;
  void readFromFile(const std::string &filename);

 private:
  void renderHelper(const Camera &camera, PPMImage &image, int startY,
                    int endY) const;
  std::vector<std::shared_ptr<SceneObject>> objects;
  std::vector<std::shared_ptr<Light>> lights;
  Camera camera;
  float t_max = 20.0f;
  Vec3 backgroundColor = {0, 0, 0};
};

#endif /* INC_SCENE */
