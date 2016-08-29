#pragma once

#include <utils/utils.h>
#include <glm/gtx/string_cast.hpp>
void drawNode(ProgramObject &program, NodeShared const&node, glm::mat4 parentMat) {
	glm::mat4 useMat = parentMat * node->getMatrix();
	program.setMatrix4fv("m", value_ptr(useMat));
	glm::mat3 n = transpose(inverse(glm::mat3(useMat)));
	program.setMatrix3fv("n", value_ptr(n));


	for (auto &m : node->meshes) {
		m->mat->bind(program);
		m->draw();
	}
	for (auto &c : node->children) {
		drawNode(program, c, useMat);
	}
};
