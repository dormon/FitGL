#pragma once
#include "Shared.h"
namespace fgl {
  classShared(Material);
  classShared(PhongMaterial);
};
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <geGLshared.h>

class fgl::Material{
public:
	virtual void bind(ge::gl::ProgramS &program) {}
};

class fgl::PhongMaterial: public fgl::Material{
public:
  PhongMaterial() {
    diffuseTex = std::make_shared<ge::gl::Texture>();
  }
	glm::vec4 diffuse = glm::vec4(0, 0, 0, 0);
  std::shared_ptr<ge::gl::Texture> diffuseTex;
	//...

	virtual void bind(ge::gl::ProgramS &program);
};
