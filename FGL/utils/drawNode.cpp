#pragma once

#include <utils/utils.h>
#include <glm/gtx/string_cast.hpp>

using namespace fgl;

void drawNode(ge::gl::ProgramS &program, fgl::NodeS const&node, glm::mat4 parentMat) {
	glm::mat4 useMat = parentMat * node->getMatrix();
	program->setMatrix4fv("m", value_ptr(useMat));
	glm::mat3 n = transpose(inverse(glm::mat3(useMat)));
	program->setMatrix3fv("n", value_ptr(n));


	for (auto &m : node->meshes) {
    m->forEach([&](GeometryS g, MaterialS m) {
      m->bind(program);
      g->draw();
    });
	}
	for (auto &c : node->children) {
		drawNode(program, c, useMat);
	}
};
