#ifndef INC_MATERIAL
#define INC_MATERIAL

#include <string>

#include "Vector.hpp"

class Material {
 public:
  Material() = default;
  Material(const std::string &id, Vec3 ambient, Vec3 diffuse, Vec3 specular,
           float phongExponent, Vec3 mirrorReflectance, float textureFactor)
      : id(id),
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        phongExponent(phongExponent),
        mirrorReflectance(mirrorReflectance),
        textureFactor(textureFactor) {
  }

  std::string id;
  Vec3 ambient;
  Vec3 diffuse;
  Vec3 specular;
  float phongExponent;
  Vec3 mirrorReflectance;
  float textureFactor;
};

#endif /* INC_MATERIAL */
