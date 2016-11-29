#pragma once
#include "Shared.h"
namespace fgl{
  classShared(Geometry);
};
#include <geGLshared.h>
#include <glm/glm.hpp>
#include <vector>

class fgl::Geometry: ge::gl::Context{
public:
  Geometry();
  virtual ~Geometry() {}

  void updateData();
  void draw();

  std::vector<GLuint> elements;
  std::vector<glm::vec3> pos;
  std::vector<glm::vec3> norm;
  std::vector<glm::vec2> tc;
protected:

  ge::gl::VertexArrayS vao;
  ge::gl::BufferS vbo;
  ge::gl::BufferS ebo;
};


