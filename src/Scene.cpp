#include "Scene.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

#include "AmbientLight.hpp"
#include "Cube.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"

void Scene::addObject(std::shared_ptr<SceneObject> object) {
  objects.push_back(std::move(object));
}

void Scene::addLight(std::shared_ptr<Light> light) {
  lights.push_back(std::move(light));
}

PPMImage Scene::render(const Camera &camera) const {
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

void Scene::renderHelper(const Camera &camera, PPMImage &image, int startY,
                         int endY) const {
  float t;
  Pixel color;
  for (int y = startY; y < endY; ++y) {
    for (int x = 0; x < image.width; ++x) {
      Ray ray;
      ray.origin = camera.position;
      ray.direction.x = (x - image.width / 2) /
                            static_cast<float>(image.height) * camera.fov /
                            180 +
                        camera.direction.x;
      ray.direction.y = (y - image.height / 2) /
                            static_cast<float>(image.height) * camera.fov /
                            180 +
                        camera.direction.y;
      ray.direction.z = -1.0f;
      Pixel pixel = {0, 0, 0};

      float t_local = t_max;
      bool hit = false;
      Vec3 surfaceNormal;
      for (const auto &object : objects) {
        if (object->intersect(ray, t, color, surfaceNormal) && t < t_local) {
          std::shared_ptr<Cube> cube = std::dynamic_pointer_cast<Cube>(object);
          pixel = color;
          t_local = t;
          hit = true;
        }
      }

      if (hit) {
        Pixel lightPixel = {0, 0, 0};
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
              float intensity =
                  std::max(0.0f,
                           std::abs(surfaceNormal.dot(shadowRay.direction))) *
                  pointLight->intensity;
              lightPixel += pointLight->color * intensity;
            }
          } else if (lightType == Light::Type::AMBIENT) {
            std::shared_ptr<AmbientLight> ambientLight =
                std::dynamic_pointer_cast<AmbientLight>(light);
            lightPixel += ambientLight->color * ambientLight->intensity;
          } else if (lightType == Light::Type::DIRECTIONAL) {
            std::shared_ptr<DirectionalLight> directionalLight =
                std::dynamic_pointer_cast<DirectionalLight>(light);
            Ray shadowRay;
            shadowRay.origin = ray.origin + ray.direction * (t_local - 0.001f);
            shadowRay.direction = -directionalLight->direction;
            float distance = std::numeric_limits<float>::max();
            shadowRay.direction = shadowRay.direction.normalize();

            if (!occluded(shadowRay, distance)) {
              float intensity =
                  std::max(0.0f,
                           std::abs(surfaceNormal.dot(shadowRay.direction))) *
                  directionalLight->intensity;
              lightPixel += directionalLight->color * intensity;
            }
          }
        }
        pixel *= lightPixel;
      }

      image.setPixel(x, y, pixel);
    }
  }
}

bool Scene::occluded(const Ray &ray, float t) const {
  return std::any_of(objects.begin(), objects.end(), [&](const auto &object) {
    float t_local;
    Pixel color;
    Vec3 surfaceNormal;
    if (object->intersect(ray, t_local, color, surfaceNormal) && t_local < t) {
      return true;
    }
    return false;
  });
}
