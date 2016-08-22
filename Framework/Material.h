#pragma once

#include <ProgramObject.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

class Material{
public:
	virtual void bind(ProgramObject &program) {}
};
typedef std::shared_ptr<Material> MaterialShared;


class PhongMaterial: public Material{
public:
	glm::vec4 diffuse;
	GLuint diffuseTex=0;
	//...

	virtual void bind(ProgramObject &program);
};
typedef std::shared_ptr<PhongMaterial> PhongMaterialShared;