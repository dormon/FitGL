#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <geGL/geGL.h>

class Material{
public:
	virtual void bind(ge::gl::Program &program) {}
};
typedef std::shared_ptr<Material> MaterialShared;


class PhongMaterial: public Material{
public:
  PhongMaterial() {
    diffuseTex = std::make_shared<ge::gl::Texture>();
  }
	glm::vec4 diffuse = glm::vec4(0, 0, 0, 0);
  std::shared_ptr<ge::gl::Texture> diffuseTex;
	//...

	virtual void bind(ge::gl::Program &program);
};
typedef std::shared_ptr<PhongMaterial> PhongMaterialShared;