#include "Material.h"

void PhongMaterial::bind(ProgramObject & program){
	program.set4fv("diffuse", glm::value_ptr(diffuse));
	glBindTextureUnit(0, diffuseTex);
}
