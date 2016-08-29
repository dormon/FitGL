#include "Mesh.h"

Mesh::Mesh(){
	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ebo);
	glCreateVertexArrays(1, &vao);
}

Mesh::~Mesh(){
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}


void Mesh::updateData(){
	int vertexCount = pos.size();

	int byteSize = vertexCount * sizeof(Vertex);
	glNamedBufferData(vbo, byteSize, 0, GL_STATIC_DRAW);
	Vertex* ptr = (Vertex*)glMapNamedBuffer(vbo, GL_WRITE_ONLY);
	for (int i = 0; i < vertexCount; i++) {
		Vertex v;
		v.pos = pos.at(i);
		if (norm.size() > 0) v.norm = norm.at(i);
		if (tc.size() > 0) v.tc = tc.at(i);
		ptr[i] = v;
	}
	glUnmapNamedBuffer(vbo);

	if (elements.size() > 0) {
		glNamedBufferData(ebo, elements.size() * sizeof(int), elements.data(), GL_STATIC_DRAW);
		glVertexArrayElementBuffer(vao, ebo);
	}
	glVertexArrayVertexBuffer(vao, 0, vbo, offsetof(Vertex, pos), sizeof(Vertex));
	glVertexArrayVertexBuffer(vao, 1, vbo, offsetof(Vertex, norm), sizeof(Vertex));
	glVertexArrayVertexBuffer(vao, 2, vbo, offsetof(Vertex, tc), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, 0, 0);
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, 0, 0);
	glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, 0, 0);
	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glEnableVertexArrayAttrib(vao, 2);

}

void Mesh::draw(){
	glBindVertexArray(vao);
	if (elements.size() > 0) {
		glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);
	}else {
		glDrawArrays(GL_TRIANGLES, 0, pos.size());
	}
}
