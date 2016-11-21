#include "Material.h"

void PhongMaterial::bind(ge::gl::Program & program){
	program.set4fv("diffuse", glm::value_ptr(diffuse));
  diffuseTex->bind(0);
}
