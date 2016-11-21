#include "VuMesh.h"

struct Vertex {
  glm::vec3 pos;
  glm::vec3 norm;
  glm::vec2 tc;
};

NeiVu::Mesh::Mesh(){
}

NeiVu::Mesh::~Mesh(){
}

void NeiVu::Mesh::updateData(){
	int vertexCount = pos.size();

	int byteSize = vertexCount * sizeof(Vertex);
  vbo = std::make_shared<Buffer>();
  vbo->alloc(byteSize, vk::BufferUsageFlagBits::eVertexBuffer);
  Vertex* ptr = (Vertex*)vbo->map();

	for (int i = 0; i < vertexCount; i++) {
		Vertex v;
		v.pos = pos.at(i);
		if (norm.size() > 0) v.norm = norm.at(i);
		if (tc.size() > 0) v.tc = tc.at(i);
		ptr[i] = v;
	}
  vbo->unmap();
  vbo->moveToGPU();

	if (elements.size() > 0) {
    ebo = std::make_shared<Buffer>();
    ebo->alloc(elements.size() * sizeof(int), vk::BufferUsageFlagBits::eIndexBuffer);
    unsigned int* ptr = (unsigned int*)ebo->map();
    for (auto &e : elements) {
      *(ptr++) = e;
    }
    ebo->unmap();
    ebo->moveToGPU();
	}
}

void NeiVu::Mesh::draw(vk::CommandBuffer cb){
  cb.bindVertexBuffers(0, { vbo->bufferGPU }, { 0 });
	if (elements.size() > 0) {
    cb.bindIndexBuffer(ebo->bufferGPU, 0, vk::IndexType::eUint32);
    cb.drawIndexed(elements.size(), 1, 0, 0, 0);
	}else {
    cb.draw(pos.size(), 1, 0, 0);
	}
}
