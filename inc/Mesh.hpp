#ifndef INC_MESH
#define INC_MESH

#include <limits>
#include <vector>

#include "Material.hpp"
#include "SceneObject.hpp"
#include "Triangle.hpp"

class Mesh : public SceneObject {
 public:
  Mesh(const std::vector<Triangle> &faces, Material material)
      : faces(faces), material(material) {
  }

  bool intersect(const Ray &ray, float &t, Vec3 &color,
                 Vec3 &surfaceNormal) const {
    bool hit = false;
    float t_local = std::numeric_limits<float>::max();
    for (const auto &face : faces) {
      if (face.intersect(ray, t, color, surfaceNormal) && t < t_local) {
        hit = true;
        t_local = t;
      }
    }
    if (hit) {
      t = t_local;
      return true;
    }
    return false;
  }

  std::vector<Triangle> faces;
  Material material;
};

#endif /* INC_MESH */
