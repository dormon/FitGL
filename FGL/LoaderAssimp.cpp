#include "Loader.h"
#include "Material.h"
#include "Mesh.h"

#include <regex>
#include <memory>

#ifdef USE_ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

using namespace fgl;

#ifdef USE_ASSIMP
void processNode(NodeS &current, aiNode *ain, std::vector<MeshS>&allMeshes) {
  for (int i = 0; i < ain->mNumMeshes; i++) {
    current->addMesh(allMeshes[ain->mMeshes[i]]);
  }

  auto mat = ain->mTransformation;
  glm::mat4 m;
  for (int i = 0; i < 16; i++)glm::value_ptr(m)[i] = mat[i / 4][i % 4];
  current->setMatrix(glm::transpose(m));

  for (int i = 0; i < ain->mNumChildren; i++) {
    auto c = ain->mChildren[i];

    NodeS n = std::make_shared<Node>();
    current->addNode(n);
    processNode(n, c, allMeshes);
  }
}
#endif

NodeS Loader::scene(std::string const & fileName) {
  NodeS node = std::make_shared<Node>();
#ifdef USE_ASSIMP
  std::vector<MaterialS> allMaterials;
  std::vector<MeshS> allMeshes;
  std::string dir = std::regex_replace(fileName, std::regex("[^/]+$"), "");

  auto importer = new Assimp::Importer();
  auto scene = importer->ReadFile(fileName.c_str(), aiProcess_Triangulate);
  if (!scene) {
    std::string er = "File not found: " + fileName + "\n";
    throw std::runtime_error(er.c_str());
    return node;
  }

  for (int i = 0; i < scene->mNumMaterials; i++) {
    auto m = std::make_shared<PhongMaterial>();
    auto mat = scene->mMaterials[i];

    aiColor3D c(0.f, 0.f, 0.f);
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, c);
    m->diffuse = glm::vec4(c.r, c.g, c.b, 1);

    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      aiString tex;
      mat->GetTexture(aiTextureType_DIFFUSE, 0, &tex);
      std::string texPath = dir + tex.C_Str();
      texPath = std::regex_replace(texPath, std::regex("\\\\"), "/");
      m->diffuseTex = Loader::texture(texPath);
      m->diffuse = glm::vec4(0, 0, 0, 0);
    }

    allMaterials.push_back(m);
  }

  for (int i = 0; i < scene->mNumMeshes; i++) {
    auto mesh = scene->mMeshes[i];

    auto m = newMesh();
    auto g = newGeometry();

    auto material = allMaterials.at(mesh->mMaterialIndex);

    g->pos.reserve(mesh->mNumVertices);
    g->norm.reserve(mesh->mNumVertices);
    g->tc.reserve(mesh->mNumVertices);

    for (int i = 0; i < mesh->mNumVertices; i++) {
      auto v = mesh->mVertices[i];
      g->pos.push_back(glm::vec3(v.x, v.y, v.z));
      auto n = mesh->mNormals[i];
      g->norm.push_back(glm::vec3(n.x, n.y, n.z));
      if (mesh->mTextureCoords[0]) {
        auto t = mesh->mTextureCoords[0][i];
        g->tc.push_back(glm::vec2(t.x, t.y));
      }
    }

    g->elements.reserve(mesh->mNumFaces * 3);
    for (int i = 0; i < mesh->mNumFaces; i++) {
      auto f = mesh->mFaces[i];
      for (int j = 0; j < f.mNumIndices; j++) {
        g->elements.push_back(f.mIndices[j]);
      }
    }

    g->updateData();
    m->addDrawable(g, material);
    allMeshes.push_back(m);
  }

  processNode(node, scene->mRootNode, allMeshes);

#endif
  return node;
}
