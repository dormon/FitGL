#include "Material.h"

using namespace fgl;

void PhongMaterial::bind(ge::gl::ProgramS & program){
	program->set4fv("diffuse", glm::value_ptr(diffuse));
  diffuseTex->bind(0);
}
