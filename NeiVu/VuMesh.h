#pragma once
#include <memory>
namespace NeiVu {
  class Mesh;
  typedef std::shared_ptr<NeiVu::Mesh> MeshShared;
};
#include "Context.h"
#include "glm/glm.hpp"
#include "Buffer.h"
#include "VuMaterial.h"

class NeiVu::Mesh: public std::enable_shared_from_this<NeiVu::Mesh> {
public:
	Mesh();
	virtual ~Mesh();

	MaterialShared mat;
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	std::vector<glm::vec2> tc;
	std::vector<unsigned int> elements;

  std::shared_ptr<Buffer> vbo;
  std::shared_ptr<Buffer> ebo;
	void updateData();
  void draw(vk::CommandBuffer cb);
};