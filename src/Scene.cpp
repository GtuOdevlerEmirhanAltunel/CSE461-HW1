#include "Scene.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <sstream>
#include <thread>

#include "AmbientLight.hpp"
#include "Cube.hpp"
#include "DirectionalLight.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "PointLight.hpp"
#include "Vector.hpp"
#include "pugixml.hpp"

void Scene::addObject(std::shared_ptr<SceneObject> object) {
  objects.push_back(std::move(object));
}

void Scene::addLight(std::shared_ptr<Light> light) {
  lights.push_back(std::move(light));
}

PPMImage Scene::render() const {
  PPMImage image(camera.width, camera.height);

  unsigned int numThreads = std::max(1u, std::thread::hardware_concurrency());
  std::vector<std::thread> threads(numThreads);
  size_t chunkSize = image.height / numThreads;

  auto start = std::chrono::high_resolution_clock::now();
  for (unsigned int i = 0; i < numThreads; ++i) {
    int startY = i * chunkSize;
    int endY = (i == numThreads - 1) ? image.height : startY + chunkSize;
    threads[i] = std::thread(&Scene::renderHelper, this, std::ref(camera),
                             std::ref(image), startY, endY);
  }

  for (auto &thread : threads) {
    thread.join();
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Render time: " << elapsed.count() << " seconds" << std::endl;
  std::cout << "--- Thread count: " << numThreads << std::endl;
  std::cout << "--- Image size: " << image.width << "x" << image.height
            << std::endl;
  std::cout << "--- Chunk size: " << chunkSize << std::endl;

  return image;
}

void Scene::readFromFile(const std::string &filename) {
  std::vector<Vec3> vertexData;
  std::vector<Material> materials;

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filename.c_str());

  if (!result) {
    std::cerr << "Error loading XML file: " << result.description()
              << std::endl;
    return;
  }

  pugi::xml_node sceneNode = doc.child("scene");
  if (!sceneNode) {
    std::cerr << "No <scene> node found in XML file." << std::endl;
    return;
  }

  for (pugi::xml_node objectNode = sceneNode.first_child(); objectNode;
       objectNode = objectNode.next_sibling()) {
    if (std::string("maxraytracedepth") == objectNode.name()) {
      t_max = objectNode.text().as_float();
    } else if (std::string("backgroundColor") == objectNode.name()) {
      backgroundColor = Vec3::parseVec3(objectNode.text().as_string());
    } else if (std::string("camera") == objectNode.name()) {
      camera.position =
          Vec3::parseVec3(objectNode.child("position").text().as_string());
      camera.direction =
          Vec3::parseVec3(objectNode.child("gaze").text().as_string());
      camera.up = Vec3::parseVec3(objectNode.child_value("up"));

      std::stringstream ss(objectNode.child_value("nearPlane"));
      ss >> camera.left >> camera.right >> camera.bottom >> camera.top;
      camera.nearDistance = std::stof(objectNode.child_value("neardistance"));

      std::stringstream res(objectNode.child_value("imageresolution"));
      res >> camera.width >> camera.height;
    } else if (std::string("lights") == objectNode.name()) {
      for (pugi::xml_node lightNode = objectNode.first_child(); lightNode;
           lightNode = lightNode.next_sibling()) {
        if (std::string("ambientlight") == lightNode.name()) {
          addLight(std::make_shared<AmbientLight>(
              Vec3::parseVec3(lightNode.text().as_string())));
        } else if (std::string("pointlight") == lightNode.name()) {
          Vec3 position =
              Vec3::parseVec3(lightNode.child("position").text().as_string());
          Vec3 intensity =
              Vec3::parseVec3(lightNode.child("intensity").text().as_string());
          addLight(std::make_shared<PointLight>(position, intensity));
        } else if (std::string("triangularlight") == lightNode.name()) {
          Vec3 v1 =
              Vec3::parseVec3(lightNode.child("vertex1").text().as_string());
          Vec3 v2 =
              Vec3::parseVec3(lightNode.child("vertex2").text().as_string());
          Vec3 v3 =
              Vec3::parseVec3(lightNode.child("vertex3").text().as_string());
          Vec3 intensity =
              Vec3::parseVec3(lightNode.child("intensity").text().as_string());
          Vec3 direction = (v2 - v1).cross(v3 - v1).normalize();
          addLight(std::make_shared<DirectionalLight>(direction, intensity));
        }
      }
    } else if (std::string("vertexdata") == objectNode.name()) {
      std::string vertexDataStr = objectNode.text().as_string();
      std::istringstream iss(vertexDataStr);
      float x, y, z;
      while (iss >> x >> y >> z) {
        vertexData.emplace_back(x, y, z);
      }
    } else if (std::string("objects") == objectNode.name()) {
      for (pugi::xml_node child = objectNode.first_child(); child;
           child = child.next_sibling()) {
        std::string materialId = child.child_value("materialid");
        auto it = std::find_if(
            materials.begin(), materials.end(),
            [&materialId](const Material &m) { return m.id == materialId; });
        if (it == materials.end()) {
          std::cerr << "Material with id " << materialId
                    << " not found in the materials list." << std::endl;
          throw std::runtime_error("Material not found");
        }

        std::vector<Triangle> triangles;
        std::string facesStr = child.child_value("faces");
        std::istringstream iss(facesStr);
        while (iss) {
          int vertexIndex[3], textureIndex[3], normalIndex[3];
          for (int i = 0; i < 3; ++i) {
            iss >> vertexIndex[i] >> textureIndex[i] >> normalIndex[i];
          }
          if (iss) {
            Vec3 v0 = vertexData[vertexIndex[0] - 1];
            Vec3 v1 = vertexData[vertexIndex[1] - 1];
            Vec3 v2 = vertexData[vertexIndex[2] - 1];
            triangles.emplace_back(v0, v1, v2, WHITE);
          }
        }
        addObject(std::make_shared<Mesh>(triangles, *it));
      }
    } else if (std::string("materials") == objectNode.name()) {
      for (pugi::xml_node materialNode = objectNode.first_child(); materialNode;
           materialNode = materialNode.next_sibling()) {
        std::string id = materialNode.attribute("id").as_string();
        Vec3 ambient = Vec3::parseVec3(materialNode.child_value("ambient"));
        Vec3 diffuse = Vec3::parseVec3(materialNode.child_value("diffuse"));
        Vec3 specular = Vec3::parseVec3(materialNode.child_value("specular"));
        float phongExponent =
            materialNode.child("phongexponent").text().as_float();
        Vec3 mirrorReflectance = Vec3::parseVec3(
            materialNode.child("mirrorreflactance").text().as_string());
        float textureFactor =
            materialNode.child("texturefactor").text().as_float();

        materials.emplace_back(id, ambient, diffuse, specular, phongExponent,
                               mirrorReflectance, textureFactor);
      }
    }
  }
}

void Scene::renderHelper(const Camera &camera, PPMImage &image, int startY,
                         int endY) const {
  float t;
  Vec3 color;

  for (int y = startY; y < endY; ++y) {
    for (int x = 0; x < image.width; ++x) {
      float ndc_x = (x + 0.5f) / image.width;
      float ndc_y = (y + 0.5f) / image.height;

      float screen_x = camera.left + (camera.right - camera.left) * ndc_x;
      float screen_y = camera.bottom + (camera.top - camera.bottom) * ndc_y;

      Vec3 forward = camera.direction.normalize();
      Vec3 right = forward.cross(camera.up).normalize();
      Vec3 up = right.cross(forward).normalize();

      Vec3 image_point = camera.position + forward * camera.nearDistance +
                         right * screen_x + up * screen_y;

      Vec3 direction = (image_point - camera.position).normalize();

      Ray ray;
      ray.origin = camera.position;
      ray.direction = direction;

      Vec3 pixel;

      float t_local = t_max;
      bool hit = false;
      Vec3 surfaceNormal;
      std::shared_ptr<Mesh> mesh = nullptr;
      for (const auto &object : objects) {
        if (object->intersect(ray, t, color, surfaceNormal) && t < t_local) {
          pixel = color;
          t_local = t;
          hit = true;
          if (auto meshPtr = std::dynamic_pointer_cast<Mesh>(object)) {
            mesh = meshPtr;
          }
        }
      }

      if (hit) {
        Vec3 lightpixel;
        for (const auto &light : lights) {
          Light::Type lightType = light->getType();
          if (lightType == Light::Type::POINT) {
            std::shared_ptr<PointLight> pointLight =
                std::dynamic_pointer_cast<PointLight>(light);
            Ray shadowRay;
            shadowRay.origin = ray.origin + ray.direction * (t_local - 0.001f);
            shadowRay.direction = pointLight->position - shadowRay.origin;
            float distance = shadowRay.direction.length();
            shadowRay.direction = shadowRay.direction.normalize();

            if (!occluded(shadowRay, distance)) {
              lightpixel +=
                  pointLight->intensity *
                  std::max(0.0f, surfaceNormal.dot(shadowRay.direction));
            }
          } else if (lightType == Light::Type::AMBIENT) {
            std::shared_ptr<AmbientLight> ambientLight =
                std::dynamic_pointer_cast<AmbientLight>(light);
            Vec3 temp = mesh->material.ambient;
            temp *= ambientLight->intensity;
            lightpixel += temp;
          } else if (lightType == Light::Type::DIRECTIONAL) {
            std::shared_ptr<DirectionalLight> directionalLight =
                std::dynamic_pointer_cast<DirectionalLight>(light);
            Ray shadowRay;
            shadowRay.origin = ray.origin + ray.direction * (t_local - 0.001f);
            shadowRay.direction = -directionalLight->direction;
            float distance = std::numeric_limits<float>::max();
            shadowRay.direction = shadowRay.direction.normalize();

            if (!occluded(shadowRay, distance)) {
              lightpixel += directionalLight->intensity *
                            std::abs(surfaceNormal.dot(shadowRay.direction));
            }
          }
        }
        pixel *= lightpixel;
      } else {
        pixel = backgroundColor;
      }

      image.setVec3(x, y, pixel);
    }
  }
}

bool Scene::occluded(const Ray &ray, float t) const {
  return std::any_of(objects.begin(), objects.end(), [&](const auto &object) {
    float t_local;
    Vec3 color;
    Vec3 surfaceNormal;
    if (object->intersect(ray, t_local, color, surfaceNormal) && t_local < t) {
      return true;
    }
    return false;
  });
}
