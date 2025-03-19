#include <iostream>
#include <string>
#include <vector>

int main() {
  // pugi::xml_document doc;
  // if (!doc.load_file("./scene.xml")) {
  //   std::cerr << "Error: Cannot load XML file.\n";
  //   return 1;
  // }

  // Camera camera;
  // std::vector<Light> lights;
  // std::vector<Material> materials;
  // std::vector<Mesh> meshes;

  // pugi::xml_node root = doc.child("scene");
  // if (!root) {
  //   std::cerr << "Error: Invalid XML structure (no <scene> root).\n";
  //   return 1;
  // }

  // parseCamera(root.child("camera"), camera);
  // parseLights(root.child("lights"), lights);
  // parseMaterials(root.child("materials"), materials);
  // parseMeshes(root.child("objects"), meshes);

  // // Print extracted data
  // std::cout << "Camera Position: " << camera.position.x << " "
  //           << camera.position.y << " " << camera.position.z << "\n";
  // std::cout << "Resolution: " << camera.imageResolution[0] << "x"
  //           << camera.imageResolution[1] << "\n";
  // std::cout << "Lights Count: " << lights.size() << "\n";
  // std::cout << "Materials Count: " << materials.size() << "\n";
  // std::cout << "Meshes Count: " << meshes.size() << "\n";

  return 0;
}
