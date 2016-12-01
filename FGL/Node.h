#pragma once
#include "Shared.h"
namespace fgl {
  classShared(Node);
}
#include <vector>
#include <glm/glm.hpp>
#include <Mesh.h>

class fgl::Node {
public:
	Node() {}
	virtual ~Node() {}

	void setMatrix(glm::mat4 const&m) { matrix = m; }
	virtual glm::mat4 getMatrix() { return matrix; }

  void addNode(NodeS node);
  void removeNode(NodeS node);

  void addMesh(MeshS &m);
  void removeMesh(MeshS &m);
	
	std::vector<NodeS> children;
	std::vector<MeshS> meshes;
  glm::mat4 matrix;
};
