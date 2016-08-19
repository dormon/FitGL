#pragma once

#include <string>
#include <GL/glew.h>
#include <memory>
#include <iostream>

class Image {
public:
	~Image() {if(data)delete data; }
	unsigned int* data=nullptr;
	int width = 0;
	int height = 0;
	GLenum format = GL_RGBA;
	GLenum type = GL_UNSIGNED_BYTE;
};
typedef std::shared_ptr<Image> ImageShared;

class Loader {
public:
	static std::string text(std::string const &fileName);
	static ImageShared image(std::string const &fileName);
	static ImageShared generateImage(int width = 512, int height = 512);
	static GLuint texture(std::string const &fileName, bool generateMipmap = true,		
		GLenum filterMin = GL_LINEAR_MIPMAP_LINEAR, GLenum filterMag=GL_LINEAR,
		GLenum wrapR = GL_REPEAT, GLenum wrapS = GL_REPEAT);

	static int generateOnImageNotFound;
};