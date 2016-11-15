#include "Buffer.h"

#include <iostream>

NeiVu::Buffer::Buffer(Context * vu):vu(vu){
  if (vu == nullptr)this->vu = NeiVu::Context::getCurrent();
}

NeiVu::Buffer::~Buffer(){
}
void NeiVu::Buffer::alloc(int size, vk::BufferUsageFlagBits usage){
  this->offset = 0;
  this->size = size;

  vk::BufferCreateInfo bci;
  bci.usage = usage;
  bci.size = size;

  buffer = vu->device.createBuffer(bci);
  
  auto mem = vu->device.getBufferMemoryRequirements(buffer);
  int index = vu->memoryTypeBitsToIndex(mem.memoryTypeBits, 
    vk::MemoryPropertyFlagBits::eHostVisible |
    vk::MemoryPropertyFlagBits::eHostCoherent);

  vk::MemoryAllocateInfo mai;
  mai.allocationSize = mem.size;
  mai.memoryTypeIndex = index;
  
  memory = vu->device.allocateMemory(mai);

  vu->device.bindBufferMemory(buffer, memory, offset);
}


void * NeiVu::Buffer::map(){
  return vu->device.mapMemory(memory, offset, size, vk::MemoryMapFlagBits());
}

void NeiVu::Buffer::unmap(){
  vu->device.unmapMemory(memory);
}
