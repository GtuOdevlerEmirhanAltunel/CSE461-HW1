#ifndef INC_TYPES
#define INC_TYPES

#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "pugixml.hpp"

struct Vec3 {
  float x, y, z;

  static Vec3 parse(const pugi::xml_node& node) {
    Vec3 v;
    sscanf(node.text().as_string(), "%f %f %f", &v.x, &v.y, &v.z);
    return v;
  }
};

struct Camera {
  Vec3 position, gaze, up;
  float nearPlane[4];
  float nearDistance;
  int imageResolution[2];

  static Camera parse(const pugi::xml_node& node) {
    Camera camera;
    camera.position = Vec3::parse(node.child("position"));
    camera.gaze = Vec3::parse(node.child("gaze"));
    camera.up = Vec3::parse(node.child("up"));

    sscanf(node.child("nearPlane").text().as_string(), "%f %f %f %f",
           &camera.nearPlane[0], &camera.nearPlane[1], &camera.nearPlane[2],
           &camera.nearPlane[3]);

    camera.nearDistance = node.child("neardistance").text().as_float();
    sscanf(node.child("imageresolution").text().as_string(), "%d %d",
           &camera.imageResolution[0], &camera.imageResolution[1]);
    return camera;
  }
};

struct AmbientLight {
  Vec3 intensity;

  static AmbientLight parse(const pugi::xml_node& node) {
    AmbientLight ambientLight;
    ambientLight.intensity = Vec3::parse(node);
    return ambientLight;
  }
};

struct PointLight {
  int id;
  Vec3 position;
  Vec3 intensity;

  static PointLight parse(const pugi::xml_node& node) {
    PointLight pointLight;
    pointLight.id = node.attribute("id").as_int();
    pointLight.position = Vec3::parse(node.child("position"));
    pointLight.intensity = Vec3::parse(node.child("intensity"));
    return pointLight;
  }
};

struct TriangularLight {
  int id;
  Vec3 vertex1;
  Vec3 vertex2;
  Vec3 vertex3;
  Vec3 intensity;

  static TriangularLight parse(const pugi::xml_node& node) {
    TriangularLight triangularLight;
    triangularLight.id = node.attribute("id").as_int();
    triangularLight.vertex1 = Vec3::parse(node.child("vertex1"));
    triangularLight.vertex2 = Vec3::parse(node.child("vertex2"));
    triangularLight.vertex3 = Vec3::parse(node.child("vertex3"));
    triangularLight.intensity = Vec3::parse(node.child("intensity"));
    return triangularLight;
  }
};

std::variant<AmbientLight, PointLight, TriangularLight> parse(
    const pugi::xml_node& node) {
  const std::string type = node.name();
  if (type == "ambientLight")
    return AmbientLight::parse(node);
  if (type == "pointlight")
    return PointLight::parse(node);
  if (type == "triangularlight")
    return TriangularLight::parse(node);

  throw "Unknown Type";
}

struct Material {
  int id;
  Vec3 ambient, diffuse, specular, mirrorReflectance;
  float phongExponent;
  float textureFactor;

  static Material parse(const pugi::xml_node& node) {
    Material material;
    material.id = node.attribute("id").as_int();
    material.ambient = Vec3::parse(node.child("ambient"));
    material.diffuse = Vec3::parse(node.child("diffuse"));
    material.specular = Vec3::parse(node.child("specular"));
    material.mirrorReflectance = Vec3::parse(node.child("mirrorreflactance"));
    material.phongExponent = node.child("phongexponent").text().as_float();
    material.textureFactor = node.child("texturefactor").text().as_float();
    return material;
  }
};

// struct Vertex

// struct Mesh {
//   int materialId;
//   std::vector<std::vector<int>> faces;

//   static Mesh parse(const pugi::xml_node& node) {
//     Mesh mesh;
//     mesh.materialId = node.child("materialid").text().as_int();
//     std::string facesText = node.child("faces").text().as_string();

//     std::istringstream faceStream(facesText);
//     std::string faceStr;
//     while (std::getline(faceStream, faceStr, ' ')) {
//       std::vector<int> face;
//       int v1, v2, v3;
//       if (sscanf(faceStr.c_str(), "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1,
//       &v2,
//                  &v3) == 3) {
//         face.push_back(v1);
//         face.push_back(v2);
//         face.push_back(v3);
//         mesh.faces.push_back(face);
//       }
//     }
//     return mesh;
//   }
// };

// struct Scene {};

#endif /* INC_TYPES */
