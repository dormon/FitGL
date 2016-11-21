#include "Buffer.h"

#include <iostream>

NeiVu::Buffer::Buffer(Context * vu):vu(vu){
  if (vu == nullptr)this->vu = NeiVu::Context::getCurrent();
}

NeiVu::Buffer::~Buffer(){
}
void NeiVu::Buffer::alloc(int size, vk::BufferUsageFlagBits usage){
  this->usage = usage|vk::BufferUsageFlagBits::eTransferSrc;
  this->offset = 0;
  this->size = size;

  vk::BufferCreateInfo bci;
  bci.usage = this->usage;
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

void NeiVu::Buffer::moveToGPU(){
  if (!bufferGPU) {
    vk::BufferCreateInfo bci;
    bci.usage = vk::BufferUsageFlagBits::eTransferDst|usage;
    bci.size = size;
    bufferGPU = vu->device.createBuffer(bci);
    auto mem = vu->device.getBufferMemoryRequirements(bufferGPU);
    int index = vu->memoryTypeBitsToIndex(mem.memoryTypeBits,
      vk::MemoryPropertyFlagBits::eDeviceLocal);
    std::cout << "moveToGPU mem index " << index << "\n";
    vk::MemoryAllocateInfo mai;
    mai.allocationSize = mem.size;
    mai.memoryTypeIndex = index;

    memoryGPU = vu->device.allocateMemory(mai);
    vu->device.bindBufferMemory(bufferGPU, memoryGPU, offset);
  }
  
  vu->commandBuffer.copyBuffer(buffer, bufferGPU, { vk::BufferCopy(0,0,size) });

}
