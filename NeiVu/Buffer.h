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

  vk::Buffer buffer;
private:
  Context* vu;
  vk::DeviceMemory memory;
  int size = 0;
  int offset = 0;
};