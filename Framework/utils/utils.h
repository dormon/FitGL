#pragma once

#include <glm/glm.hpp>
#include <ProgramObject.h>
#include <Node.h>

void drawNode(ProgramObject &program, NodeShared const&node, glm::mat4 parentMat = glm::mat4());
