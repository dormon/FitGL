#pragma once
namespace fgl {
  class Loader;
};
#include <string>
#include <geGL/geGL.h>
#include <Image.h>
#include <Node.h>


class fgl::Loader {
public:
  static std::vector<char> binary(std::string const &fileName);
	// text
	static std::string text(std::string const &fileName);
	// textures
	static std::shared_ptr<ge::gl::Texture> texture(std::string const & fileName, bool generateMipmap=true,
		GLenum filterMin=GL_LINEAR_MIPMAP_LINEAR, GLenum filterMag=GL_LINEAR,
		GLenum wrapR=GL_REPEAT, GLenum wrapS=GL_REPEAT);
	static std::shared_ptr<ge::gl::Texture> cubemap(std::string const(&fileNames)[6], bool generateMipmap = true,
		GLenum filterMin = GL_LINEAR_MIPMAP_LINEAR, GLenum filterMag = GL_LINEAR,
		GLenum wrapR = GL_REPEAT, GLenum wrapS = GL_REPEAT);
	static ImageS image(std::string const &fileName);
	static ImageS generateImage(int width = 512, int height = 512);
	// Scene
	static NodeS scene(std::string const & fileName);

	static bool generateOnImageNotFound;
};