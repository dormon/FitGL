#pragma once
namespace NeiVu {
  class Texture;
};

#include "Context.h"

class NeiVu::Texture {
public:
  Texture(Context* vu=nullptr);
  ~Texture();

  void alloc(int width, int height, vk::Format format);
  void setReadOptimal();
  void* map();
  void unmap();

  vk::Image image;
  vk::ImageView imageView;
  vk::Sampler sampler;
private:
  Context* vu;
  vk::DeviceMemory memory;
  vk::DeviceSize size;

  int width = 0;
  int height = 0;
};