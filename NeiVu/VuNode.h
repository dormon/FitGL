#pragma once
#include <memory>
namespace NeiVu {
  class Node;
  typedef std::shared_ptr<Node> NodeShared;
};

#include <vector>

#include <glm/glm.hpp>
#include <VuMesh.h>


class NeiVu::Node {
public:
	Node() {}
	virtual ~Node() {}

	void setMatrix(glm::mat4 const&m) { matrix = m; }
	virtual glm::mat4 getMatrix() { return matrix; }
	void addNode(NodeShared node) { children.push_back(node); }
	void removeNode(NodeShared node);

	void addMesh(MeshShared &m) { meshes.push_back(m); }
	
	glm::mat4 matrix;
	std::vector<NodeShared> children;
	std::vector<MeshShared> meshes;
};
