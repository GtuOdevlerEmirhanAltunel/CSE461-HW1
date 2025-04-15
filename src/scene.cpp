#include "scene.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <mutex>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "pugixml.hpp"
#include "stb_image.h"

static Vec3 parseVec3(const pugi::xml_text& text) {
  Vec3 vec;
  std::istringstream iss(text.as_string());
  iss >> vec.x >> vec.y >> vec.z;
  return vec;
}

static void parseVec4(const pugi::xml_text& text, Number* vec) {
  std::istringstream iss(text.as_string());
  for (int i = 0; i < 4; ++i) {
    iss >> vec[i];
  }
}

Scene::Scene(const std::string& fileName) {
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(fileName.c_str());

  if (!result) {
    std::cerr << "Error loading XML file: " << result.description()
              << std::endl;
    throw std::runtime_error("Failed to load XML file.");
  }

  pugi::xml_node sceneNode = doc.child("scene");
  if (!sceneNode) {
    std::cerr << "No <scene> node found in XML file." << std::endl;
    throw std::runtime_error("No <scene> node found in XML file.");
  }

  this->_max_ray_trace_depth =
      sceneNode.child("maxraytracedepth").text().as_float();
  this->_background_color =
      parseVec3(sceneNode.child("backgroundColor").text()) / 255.0f;
  auto cameraNode = sceneNode.child("camera");
  this->_camera = Camera{parseVec3(cameraNode.child("position").text()),
                         parseVec3(cameraNode.child("gaze").text()),
                         parseVec3(cameraNode.child("up").text()),
                         {0, 0, 0, 0},
                         cameraNode.child("neardistance").text().as_float(),
                         0,
                         0};
  parseVec4(cameraNode.child("nearPlane").text(), this->_camera.near_plane);
  std::string imageResolutionNodeText =
      cameraNode.child("imageresolution").text().as_string();
  std::istringstream imageResolutionStream(imageResolutionNodeText);
  imageResolutionStream >> this->_camera.width;
  imageResolutionStream >> this->_camera.height;

  auto lightsNode = sceneNode.child("lights");

  for (auto lightNode : lightsNode.children()) {
    std::string type = lightNode.name();
    if (type == "ambientlight") {
      this->_ambientLight.intensity = parseVec3(lightNode.text()) / 255.0f;
    } else if (type == "pointlight") {
      PointLight pointLight;
      pointLight.id = lightNode.attribute("id").as_string();
      pointLight.intensity =
          parseVec3(lightNode.child("intensity").text()) / 1000.0f;
      pointLight.position = parseVec3(lightNode.child("position").text());
      this->_lights.push_back(std::make_shared<PointLight>(pointLight));
    } else if (type == "triangularlight") {
      TriangularLight triangularLight;
      triangularLight.id = lightNode.attribute("id").as_string();
      triangularLight.intensity =
          parseVec3(lightNode.child("intensity").text()) / 1000.0f;
      triangularLight.vertexes[0] =
          parseVec3(lightNode.child("vertex1").text());
      triangularLight.vertexes[1] =
          parseVec3(lightNode.child("vertex2").text());
      triangularLight.vertexes[2] =
          parseVec3(lightNode.child("vertex3").text());
      triangularLight.calculateDirection();
      this->_lights.push_back(
          std::make_shared<TriangularLight>(triangularLight));
    }
  }

  auto materialsNode = sceneNode.child("materials");

  for (auto materialNode : materialsNode.children()) {
    Material material;
    material.id = materialNode.attribute("id").as_string();
    material.ambient = parseVec3(materialNode.child("ambient").text());
    material.diffuse = parseVec3(materialNode.child("diffuse").text());
    material.specular = parseVec3(materialNode.child("specular").text());
    material.mirror_reflectance =
        parseVec3(materialNode.child("mirrorreflactance").text());
    material.phong_exponent =
        materialNode.child("phongexponent").text().as_float();
    material.texture_factor =
        materialNode.child("texturefactor").text().as_float();
    this->_materials.push_back(material);
  }

  auto vertexDataNode = sceneNode.child("vertexdata");
  std::string vertexData = vertexDataNode.text().as_string();
  std::istringstream vertexStream(vertexData);
  Vec3 vertexMin(std::numeric_limits<Number>::max(),
                 std::numeric_limits<Number>::max(),
                 std::numeric_limits<Number>::max());
  Vec3 vertexMax(std::numeric_limits<Number>::lowest(),
                 std::numeric_limits<Number>::lowest(),
                 std::numeric_limits<Number>::lowest());
  while (vertexStream) {
    Vec3 vertex;
    vertexStream >> vertex.x >> vertex.y >> vertex.z;
    if (vertexStream) {
      vertexMin.x = std::min(vertexMin.x, vertex.x);
      vertexMin.y = std::min(vertexMin.y, vertex.y);
      vertexMin.z = std::min(vertexMin.z, vertex.z);
      vertexMax.x = std::max(vertexMax.x, vertex.x);
      vertexMax.y = std::max(vertexMax.y, vertex.y);
      vertexMax.z = std::max(vertexMax.z, vertex.z);
      this->_vertex_data.push_back(vertex);
    }
  }

  auto textureDataNode = sceneNode.child("texturedata");
  std::string textureData = textureDataNode.text().as_string();
  std::istringstream textureStream(textureData);
  while (textureStream) {
    Number u, v;
    textureStream >> u >> v;
    if (textureStream) {
      this->_texture_data.emplace_back(u, v);
    }
  }

  auto normalDataNode = sceneNode.child("normaldata");
  std::string normalData = normalDataNode.text().as_string();
  std::istringstream normalStream(normalData);
  while (normalStream) {
    Vec3 normal;
    normalStream >> normal.x >> normal.y >> normal.z;
    if (normalStream) {
      this->_normal_data.push_back(normal);
    }
  }

  this->_texture_file = sceneNode.child("textureimage").text().as_string();
  int width, height, channels;
  unsigned char* data =
      stbi_load(this->_texture_file.c_str(), &width, &height, &channels, 0);
  if (data) {
    this->_texture_image.resize(height, std::vector<Vec3>(width));
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        int index = (y * width + x) * channels;
        this->_texture_image[y][x] =
            Vec3(data[index], data[index + 1], data[index + 2]) / 255.0f;
      }
    }
    stbi_image_free(data);
  } else {
    std::cerr << "Error loading texture image: " << stbi_failure_reason()
              << std::endl;
    throw std::runtime_error("Failed to load texture image.");
  }

  auto meshesNode = sceneNode.child("objects");
  for (auto meshNode : meshesNode.children()) {
    Mesh mesh;
    mesh.id = meshNode.attribute("id").as_string();
    mesh.material_id = meshNode.child("materialid").text().as_string();
    std::string facesData = meshNode.child("faces").text().as_string();
    std::istringstream facesStream(facesData);
    std::string firstLine;
    std::getline(facesStream, firstLine);
    while (firstLine.empty()) {
      std::getline(facesStream, firstLine);
    }
    facesStream.seekg(0, std::ios::beg);
    bool hasTexture = false;
    if (firstLine.find("/") != std::string::npos) {
      hasTexture = true;
    }
    while (facesStream) {
      if (facesStream.eof()) break;
      Face face;
      for (int i = 0; i < 3; ++i) {
        facesStream >> face.vertex_indices[i];
        if (hasTexture) {
          facesStream.ignore(1);
          facesStream >> face.uv_indices[i];
          facesStream.ignore(1);
          facesStream >> face.normal_indices[i];
        } else {
          face.uv_indices[i] = 0;
          face.normal_indices[i] = 0;
        }
      }
      mesh.faces.push_back(face);
      mesh.bounding_box_min = vertexMin;
      mesh.bounding_box_max = vertexMax;
    }
    this->_meshes.push_back(mesh);
  }
}

Image Scene::render() const {
  Image image(this->_camera.width, this->_camera.height);
  std::vector<std::thread> threads;
  auto threadCount = std::thread::hardware_concurrency();
  std::vector<int> threadStatus(threadCount, 0);
  auto startTime = std::chrono::high_resolution_clock::now();

  std::queue<size_t> rowsToProcess;
  for (size_t y = 0; y < this->_camera.height; ++y) {
    rowsToProcess.push(y);
  }

  bool allDone = false;
  std::thread statusThread([&]() {
    while (!allDone) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::cout << "\033[2K";
      std::cout << "Remaining rows: " << std::setw(4) << rowsToProcess.size()
                << " | ";
      std::cout << "Thread status: ";
      for (size_t i = 0; i < threadCount; ++i) {
        std::cout << std::setw(3) << threadStatus[i] << " ";
      }
      std::cout << "\r" << std::flush;
    }
    allDone = true;
    std::cout << "All threads are done." << std::endl;
  });
  std::mutex mutex;

  for (size_t threadId = 0; threadId < threadCount; ++threadId) {
    threads.emplace_back(
        [threadId, &mutex, &rowsToProcess, this, &threadStatus, &image]() {
          while (true) {
            constexpr size_t maxRows = 1;
            size_t rows[maxRows];
            size_t ri = 0;
            {
              std::lock_guard<std::mutex> lock(mutex);
              if (rowsToProcess.empty()) break;
              for (ri = 0; ri < maxRows && !rowsToProcess.empty(); ++ri) {
                rows[ri] = rowsToProcess.front();
                rowsToProcess.pop();
              }
            }

            for (size_t i = 0; i < ri; ++i) {
              size_t row = rows[i];

              for (size_t x = 0; x < this->_camera.width; ++x) {
                Number t;
                Vec3 color;
                Ray ray = this->_camera.generateRay(x, row);
                this->_traceRay(ray, t, color);
                image.setPixel(x, row, color);
              }
              threadStatus[threadId]++;
            }
          }
        });
  }

  for (auto& thread : threads) {
    thread.join();
  }

  allDone = true;
  statusThread.join();
  auto endTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      endTime - startTime);
  std::cout << "Rendering completed in " << duration.count() << " ms."
            << std::endl;
  std::cout << "Image size: " << image.width << "x" << image.height
            << std::endl;
  std::cout << "Thread count: " << threadCount << std::endl;
  return image;
}

static bool intersectRayAABB(const Ray& ray, const Vec3& min, const Vec3& max) {
  Number tmin = (min.x - ray.origin.x) / ray.direction.x;
  Number tmax = (max.x - ray.origin.x) / ray.direction.x;
  if (tmin > tmax) std::swap(tmin, tmax);

  Number tymin = (min.y - ray.origin.y) / ray.direction.y;
  Number tymax = (max.y - ray.origin.y) / ray.direction.y;
  if (tymin > tymax) std::swap(tymin, tymax);

  if ((tmin > tymax) || (tymin > tmax)) return false;

  if (tymin > tmin) tmin = tymin;
  if (tymax < tmax) tmax = tymax;

  Number tzmin = (min.z - ray.origin.z) / ray.direction.z;
  Number tzmax = (max.z - ray.origin.z) / ray.direction.z;
  if (tzmin > tzmax) std::swap(tzmin, tzmax);

  if ((tmin > tzmax) || (tzmin > tmax)) return false;

  return true;
}

static bool intersectRayTriangle(Ray ray, const Vec3& v0, const Vec3& v1,
                                 const Vec3& v2, Number& t, Number& u,
                                 Number& v) {
  Vec3 edge1 = v1 - v0;
  Vec3 edge2 = v2 - v0;
  Vec3 h = cross(ray.direction, edge2);
  Number a = dot(edge1, h);

  if (a > -EPSILON && a < EPSILON) return false;

  Number f = 1.0f / a;
  Vec3 s = ray.origin - v0;
  u = f * dot(s, h);

  if (u < 0.0f || u > 1.0f) return false;

  Vec3 q = cross(s, edge1);
  v = f * dot(ray.direction, q);

  if (v < 0.0f || u + v > 1.0f) return false;

  t = f * dot(edge2, q);

  return t > EPSILON;
}

bool Scene::_traceRayHelper(const Ray& ray, Number& t, Vec3* normal,
                            const Mesh** closestMesh,
                            const Face** closestFace) const {
  bool hit = false;
  for (const auto& mesh : _meshes) {
    if (!intersectRayAABB(ray, mesh.bounding_box_min, mesh.bounding_box_max)) {
      continue;
    }

    for (const auto& face : mesh.faces) {
      const Vec3& v0 = _vertex_data[face.vertex_indices[0] - 1];
      const Vec3& v1 = _vertex_data[face.vertex_indices[1] - 1];
      const Vec3& v2 = _vertex_data[face.vertex_indices[2] - 1];

      Number tTemp, u, v;
      if (intersectRayTriangle(ray, v0, v1, v2, tTemp, u, v)) {
        if (tTemp < t && tTemp > 0) {
          t = tTemp;
          hit = true;
          if (closestMesh != nullptr) {
            *closestMesh = &mesh;
          }
          if (closestFace != nullptr) {
            *closestFace = &face;
          }
          if (normal != nullptr) {
            *normal = _normal_data[face.normal_indices[0] - 1];
          }
        }
      }
    }
  }
  return hit;
}

static Vec3 calculateBarycentric(const Vec3& p, const Vec3& v0, const Vec3& v1,
                                 const Vec3& v2) {
  Vec3 v0v1 = v1 - v0;
  Vec3 v0v2 = v2 - v0;
  Vec3 p0v0 = p - v0;

  Number d00 = dot(v0v1, v0v1);
  Number d01 = dot(v0v1, v0v2);
  Number d11 = dot(v0v2, v0v2);
  Number d20 = dot(p0v0, v0v1);
  Number d21 = dot(p0v0, v0v2);

  Number denom = d00 * d11 - d01 * d01;
  if (denom == 0) return Vec3(0.0f, 0.0f, 0.0f);
  Number v = (d11 * d20 - d01 * d21) / denom;
  Number w = (d00 * d21 - d01 * d20) / denom;

  return Vec3(1.0f - v - w, v, w);
}

bool Scene::_traceRay(const Ray& ray, Number& t, Vec3& color,
                      int reflectDepth) const {
  if (reflectDepth > _max_ray_trace_depth) {
    color = this->_background_color;
    return false;
  }
  bool hit = false;
  const Mesh* closestMesh = nullptr;
  const Face* closestFace = nullptr;
  Vec3 normal;
  t = std::numeric_limits<Number>::max();

  hit = _traceRayHelper(ray, t, &normal, &closestMesh, &closestFace);

  if (hit) {
    auto materialIt =
        std::find_if(_materials.begin(), _materials.end(),
                     [&closestMesh](const Material& material) {
                       return material.id == closestMesh->material_id;
                     });

    color = _calculateLighting(ray.origin + ray.direction * (t - EPSILON),
                               normal, *materialIt, ray.direction);

    auto reflectRay = Ray{ray.origin + ray.direction * (t - EPSILON),
                          reflect(ray.direction, normal)};
    Vec3 reflectColor;
    Number reflectT;
    bool reflectHit =
        _traceRay(reflectRay, reflectT, reflectColor, reflectDepth + 1);
    color += reflectColor * materialIt->mirror_reflectance;

    auto barycentric =
        calculateBarycentric(ray.origin + ray.direction * (t - EPSILON),
                             _vertex_data[closestFace->vertex_indices[0] - 1],
                             _vertex_data[closestFace->vertex_indices[1] - 1],
                             _vertex_data[closestFace->vertex_indices[2] - 1]);

    Vec2 uv = _texture_data[closestFace->uv_indices[0] - 1] * barycentric.x +
              _texture_data[closestFace->uv_indices[1] - 1] * barycentric.y +
              _texture_data[closestFace->uv_indices[2] - 1] * barycentric.z;
    Vec2 texCoord = Vec2(uv.u * (_texture_image[0].size() - 1),
                         uv.v * (_texture_image.size() - 1));
    Vec3 texColor = _texture_image[static_cast<size_t>(texCoord.v)]
                                  [static_cast<size_t>(texCoord.u)];
    color = color * (1.0f - materialIt->texture_factor) +
            texColor * materialIt->texture_factor;

  } else {
    color = this->_background_color;
  }

  return hit;
}

Vec3 Scene::_calculateLighting(const Vec3& hitPoint, const Vec3& normal,
                               const Material& material,
                               const Vec3& viewDir) const {
  Vec3 color(0.0f, 0.0f, 0.0f);

  color += material.ambient * _ambientLight.intensity;

  for (const auto& light : _lights) {
    Vec3 lightDir = light->getDirection(hitPoint);
    Number lightDist = light->getDistance(hitPoint);

    Ray shadowRay = {hitPoint + normal * EPSILON, lightDir};
    Number tShadow = std::numeric_limits<Number>::max();
    bool isInShadow = _traceRayHelper(shadowRay, tShadow);

    if (!isInShadow) {
      auto attenuation =
          1.0f / (1.0f + 0.2f * lightDist + 0.2f * lightDist * lightDist);
      Number diffuseIntensity = std::max(dot(normal, lightDir), 0.0f);
      Vec3 diffuse =
          material.diffuse * diffuseIntensity * light->intensity * attenuation;

      Vec3 reflectDir = reflect(lightDir, normal);
      Number specAngle = std::max(dot(viewDir, reflectDir), 0.0f);
      Vec3 specular = material.specular *
                      std::pow(specAngle, material.phong_exponent) *
                      light->intensity * attenuation;

      color += diffuse + specular;
    }
  }

  return color;
}
