#pragma once
namespace NeiVu {
  class Buffer;
};

#include "Context.h"

class NeiVu::Buffer {
public:
  Buffer(Context* vu=nullptr);
  ~Buffer();

  void alloc(int size, vk::BufferUsageFlagBits usage);
  void* map();
  void unmap();
  void moveToGPU();

  vk::Buffer buffer;
  vk::Buffer bufferGPU;
private:
  Context* vu;
  vk::DeviceMemory memory;
  vk::DeviceMemory memoryGPU;
  int size = 0;
  int offset = 0;
  vk::BufferUsageFlags usage;
};