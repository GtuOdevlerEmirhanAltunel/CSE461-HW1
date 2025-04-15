#ifndef INC_SCENE
#define INC_SCENE

#include <memory>
#include <string>
#include <vector>

#include "math.hpp"

struct Camera {
  Vec3 position;
  Vec3 gaze;
  Vec3 up;
  Number near_plane[4];
  Number near_distance;
  Index width;
  Index height;

  Ray generateRay(Index x, Index y) const {
    Vec3 w = normalize(gaze * -1.0f);
    Vec3 u = normalize(cross(up, w));
    Vec3 v = cross(w, u);

    Number l = near_plane[0];
    Number r = near_plane[1];
    Number b = near_plane[2];
    Number t = near_plane[3];

    Number uS = l + (r - l) * ((x + 0.5f) / width);
    Number vS = b + (t - b) * ((y + 0.5f) / height);

    Vec3 rayDir = normalize(u * uS + v * vS - w * near_distance);

    return Ray{position, rayDir};
  }
};

struct AmbientLight {
  Vec3 intensity;
};

struct Light {
  std::string id;
  Vec3 intensity;

  virtual Vec3 getDirection(const Vec3& hitPoint) const = 0;
  virtual Number getDistance(const Vec3& hitPoint) const = 0;
};

struct PointLight : public Light {
  Vec3 position;

  Vec3 getDirection(const Vec3& hitPoint) const override {
    return normalize(position - hitPoint);
  }

  Number getDistance(const Vec3& hitPoint) const override {
    return length(position - hitPoint);
  }
};

struct TriangularLight : public Light {
  Vec3 vertexes[3];
  Vec3 direction;

  Vec3 getDirection(const Vec3& hitPoint) const override {
    (void)hitPoint;
    return direction;
  }

  Number getDistance(const Vec3& hitPoint) const override {
    (void)hitPoint;
    return 1.0f;
  }

  void calculateDirection() {
    direction =
        normalize(cross(vertexes[0] - vertexes[1], vertexes[0] - vertexes[2]));
  }
};

struct Material {
  std::string id;
  Vec3 ambient;
  Vec3 diffuse;
  Vec3 specular;
  Vec3 mirror_reflectance;
  Number phong_exponent;
  Number texture_factor;
};

struct Face {
  Index vertex_indices[3];
  Index uv_indices[3];
  Index normal_indices[3];
};

struct Mesh {
  std::string id;
  std::string material_id;
  std::vector<Face> faces;
  Vec3 bounding_box_min;
  Vec3 bounding_box_max;
};

struct Image {
  Index width;
  Index height;
  std::vector<unsigned char[3]> data;

  Image(Index width, Index height)
      : width(width), height(height), data(width * height) {}

  static unsigned char convertToByte(Number value) {
    if (value < 0) value = 0;
    if (value > 1.0f) value = 1.0f;
    return static_cast<unsigned char>(value * 255);
  }

  inline void setPixel(Index x, Index y, Vec3 color) {
    color = color ^ 2.2f;
    if (color.x < 0) color.x = 0;
    if (color.x > 1.0f) color.x = 1.0f;
    if (color.y < 0) color.y = 0;
    if (color.y > 1.0f) color.y = 1.0f;
    if (color.z < 0) color.z = 0;
    if (color.z > 1.0f) color.z = 1.0f;
    data[y * width + x][0] = static_cast<unsigned char>(color.x * 255);
    data[y * width + x][1] = static_cast<unsigned char>(color.y * 255);
    data[y * width + x][2] = static_cast<unsigned char>(color.z * 255);
  }
};

class Scene {
 public:
  Scene() = default;
  Scene(const std::string& fileName);

  Image render() const;

 private:
  bool _traceRayHelper(const Ray& ray, Number& t, Vec3* normal = 0,
                       const Mesh** closestMesh = 0,
                       const Face** closestFace = 0) const;
  bool _traceRay(const Ray& ray, Number& t, Vec3& color,
                 int reflectDepth = 0) const;
  Vec3 _calculateLighting(const Vec3& hitPoint, const Vec3& normal,
                          const Material& material, const Vec3& viewDir) const;

 private:
  Number _max_ray_trace_depth = 5;
  Vec3 _background_color = Vec3(0.0f, 0.0f, 0.0f);
  Camera _camera = Camera();
  AmbientLight _ambientLight;
  std::vector<std::shared_ptr<Light>> _lights;
  std::vector<Material> _materials;
  std::vector<Vec3> _vertex_data;
  std::vector<Vec2> _texture_data;
  std::vector<Vec3> _normal_data;
  std::string _texture_file;
  std::vector<std::vector<Vec3>> _texture_image;
  std::vector<Mesh> _meshes;
};

#endif /* INC_SCENE */
