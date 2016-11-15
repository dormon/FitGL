#pragma once

#include <string>
#include <GL/glew.h>
#include <Image.h>
#include <Node.h>


class Loader {
public:
  static std::vector<char> binary(std::string const &fileName);
	// text
	static std::string text(std::string const &fileName);
	// textures
	static GLuint texture(std::string const & fileName, bool generateMipmap=true,
		GLenum filterMin=GL_LINEAR_MIPMAP_LINEAR, GLenum filterMag=GL_LINEAR,
		GLenum wrapR=GL_REPEAT, GLenum wrapS=GL_REPEAT);
	static GLuint cubemap(std::string const(&fileNames)[6], bool generateMipmap = true,
		GLenum filterMin = GL_LINEAR_MIPMAP_LINEAR, GLenum filterMag = GL_LINEAR,
		GLenum wrapR = GL_REPEAT, GLenum wrapS = GL_REPEAT);
	static ImageShared image(std::string const &fileName);
	static ImageShared generateImage(int width = 512, int height = 512);
	// Scene
	static NodeShared scene(std::string const & fileName);

	static bool generateOnImageNotFound;
};