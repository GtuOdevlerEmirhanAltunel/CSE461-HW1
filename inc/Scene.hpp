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
  void addObject(std::shared_ptr<SceneObject> object);
  void addLight(std::shared_ptr<Light> light);
  PPMImage render(const Camera &camera) const;
  bool occluded(const Ray &ray, float t) const;

 private:
  void renderHelper(const Camera &camera, PPMImage &image, int startY,
                    int endY) const;
  std::vector<std::shared_ptr<SceneObject>> objects;
  std::vector<std::shared_ptr<Light>> lights;
  float t_max = 20.0f;
};

#endif /* INC_SCENE */
