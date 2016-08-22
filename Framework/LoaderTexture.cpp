#include "Loader.h"

#include <iostream>

#ifdef USE_FREEIMAGE
#include <FreeImagePlus.h>
#endif

bool Loader::generateOnImageNotFound = true;

GLuint Loader::texture(std::string const & fileName, bool generateMipmap,
	GLenum filterMin, GLenum filterMag, GLenum wrapR, GLenum wrapS) {
	auto img = image(fileName);
	GLuint tex;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureImage2DEXT(tex, GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, img->format, img->type, img->data);
	if (generateMipmap)glGenerateTextureMipmap(tex);
	glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, filterMin);
	glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, filterMag);
	glTextureParameteri(tex, GL_TEXTURE_WRAP_R, wrapR);
	glTextureParameteri(tex, GL_TEXTURE_WRAP_S, wrapS);
	return tex;
}


ImageShared Loader::image(std::string const & fileName) {
#ifdef USE_FREEIMAGE
	fipImage fimg;
	if (!fimg.load(fileName.c_str())) {
		std::string er = "File not found or unknown format: " + fileName;
		if (generateOnImageNotFound) {
			std::cerr << er << "\n";
			return generateImage();
		}
		else {
			throw std::exception(er.c_str());
			return nullptr;
		}
	}
	fimg.convertTo32Bits();
	auto img = std::make_shared<Image>();
	img->height = fimg.getHeight();
	img->width = fimg.getWidth();
	img->data = new unsigned int[img->height*img->width];
	img->format = GL_BGRA;
	memcpy(img->data, fimg.accessPixels(), sizeof(int)*img->width*img->height);
	return img;
#else
	// generate random color checkboard
	return generateImage();
#endif
}


ImageShared Loader::generateImage(int width, int height) {
	auto img = std::make_shared<Image>();
	img->width = width;
	img->height = height;
	img->data = new unsigned int[width * height];
	unsigned int colors[] = {
		0xFF000000,
		0xFFFF0000,
		0xFF00FF00,
		0xFF0000FF,
		0xFFFFFF00,
		0xFFFF00FF,
		0xFF00FFFF,
		0xFFFFFFFF
	};
	int c1 = rand() % 8;
	int c2 = rand() % 8;
	if (c2 == c1)c2 = (c2 + 1) % 8;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++) {
			if (((x >> 4) & 1) == ((y >> 4) & 1))
				img->data[y * width + x] = colors[c1];
			else
				img->data[y * width + x] = colors[c2];
		}
	return img;
}
