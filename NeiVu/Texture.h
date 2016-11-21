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
  void moveToGPU();

  vk::Image image;
  vk::ImageView imageView;
  vk::Sampler sampler;

  vk::Image imageGPU;
private:
  Context* vu;
  vk::DeviceMemory memory;
  vk::DeviceMemory memoryGPU;
  vk::DeviceSize size;
  
  vk::Format format;
  int width = 0;
  int height = 0;
};