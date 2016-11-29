#pragma once

#include <glm/glm.hpp>
#include <geGLshared.h>
#include <Node.h>

void drawNode(ge::gl::ProgramS &program, fgl::NodeS const&node, glm::mat4 parentMat = glm::mat4());

template<typename...ARGS>
ge::gl::ShaderS compileShader(GLenum type, ARGS... sources) {
  return ge::gl::newShader(type,sources...);
}

template<typename...ARGS>
ge::gl::ProgramS createProgram(ARGS...args) {
  auto prog = ge::gl::newProgram(args...);
  if (!prog->getLinkStatus()) {
    throw std::runtime_error("Program link failed!");
  }
  return prog;
}