#pragma once

#include <Material.h>

#include <vector>
#include <map>
#include <GL/glew.h>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 tc;
};

class Mesh{
public:
	Mesh();
	virtual ~Mesh();


	MaterialShared mat;
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	std::vector<glm::vec2> tc;
	std::vector<GLuint> elements;

	GLuint vao=0;
	GLuint vbo=0;
	GLuint ebo=0;
	void updateData();
	void draw();
};
typedef std::shared_ptr<Mesh> MeshShared;