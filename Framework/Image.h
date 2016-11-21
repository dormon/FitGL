#pragma once
#include <memory>
#include <geGL/geGL.h>

class Image {
public:
	~Image() { if (data)delete[]data; }
	unsigned int* data = nullptr;
	int width = 0;
	int height = 0;
	GLenum format = GL_RGBA;
	GLenum type = GL_UNSIGNED_BYTE;
};
typedef std::shared_ptr<Image> ImageShared;
