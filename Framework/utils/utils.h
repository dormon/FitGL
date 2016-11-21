#pragma once

#include <glm/glm.hpp>
#include <geGL/geGL.h>
#include <Node.h>

void drawNode(ge::gl::Program &program, NodeShared const&node, glm::mat4 parentMat = glm::mat4());

template<typename...ARGS>
std::shared_ptr<ge::gl::Shader> compileShader(GLenum type, ARGS... sources) {
  return std::make_shared<ge::gl::Shader>(type,sources...);
}

template<typename...ARGS>
std::shared_ptr<ge::gl::Program> createProgram(ARGS...args) {
  auto prog = std::make_shared<ge::gl::Program>(args...);
  if (!prog->getLinkStatus()) {
    throw std::runtime_error("Program link failed!");
  }
  return prog;
}