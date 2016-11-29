#include "Geometry.h"


using namespace fgl;

struct Vertex {
  glm::vec3 pos;
  glm::vec3 norm;
  glm::vec2 tc;
};

fgl::Geometry::Geometry() {
  vao = ge::gl::newVertexArray();
  vbo = ge::gl::newBuffer();
  ebo = ge::gl::newBuffer();
}

void fgl::Geometry::updateData(){
  int vertexCount = pos.size();
  int byteSize = vertexCount * sizeof(Vertex);
  vbo->alloc(byteSize);
  
  Vertex* ptr = (Vertex*)vbo->map(GL_WRITE_ONLY);
  for (int i = 0; i < vertexCount; i++) {
    Vertex v;
    v.pos = pos.at(i);
    if (norm.size() > 0) v.norm = norm.at(i);
    if (tc.size() > 0) v.tc = tc.at(i);
    ptr[i] = v;
  }
  vbo->unmap();

  if (elements.size() > 0) {
    ebo->alloc(sizeof(int)*elements.size(), elements.data());
    vao->addElementBuffer(ebo);
  }

  vao->addAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, pos));
  vao->addAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, norm));
  vao->addAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tc));

}

void fgl::Geometry::draw(){
  vao->bind();
  if (elements.size() > 0) {
    glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);
  }  else {
    glDrawArrays(GL_TRIANGLES, 0, pos.size());
  }
}
