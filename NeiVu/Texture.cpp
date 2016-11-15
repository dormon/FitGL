#include "Texture.h"

#include <iostream>

NeiVu::Texture::Texture(Context * vu) :vu(vu) {
  if (vu == nullptr)this->vu = NeiVu::Context::getCurrent();
}

NeiVu::Texture::~Texture() {
}

void NeiVu::Texture::alloc(int width, int height, vk::Format format) {
  std::cout << "Texture::alloc " << width << " " << height << vk::to_string(format) << "\n";
  vk::ImageCreateInfo ici;
  ici.arrayLayers = 1;
  ici.extent = vk::Extent3D(width, height, 1);
  ici.format = format;
  ici.imageType = vk::ImageType::e2D;
  ici.initialLayout = vk::ImageLayout::ePreinitialized;
  ici.mipLevels = 1;
  ici.tiling = vk::ImageTiling::eLinear;
  ici.usage = vk::ImageUsageFlagBits::eSampled;

  image = vu->device.createImage(ici);

  auto mem = vu->device.getImageMemoryRequirements(image);
  size = mem.size;
  int index = vu->memoryTypeBitsToIndex(mem.memoryTypeBits,
    vk::MemoryPropertyFlagBits::eHostVisible |
    vk::MemoryPropertyFlagBits::eHostCoherent);

  vk::MemoryAllocateInfo mai;
  mai.allocationSize = mem.size;
  mai.memoryTypeIndex = index;
  memory = vu->device.allocateMemory(mai);

  vu->device.bindImageMemory(image, memory, 0);


  vk::ImageViewCreateInfo ivci;
  ivci.image = image;
  ivci.viewType = vk::ImageViewType::e2D;
  ivci.format = format;
  ivci.subresourceRange = vk::ImageSubresourceRange(
    vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

  imageView = vu->device.createImageView(ivci);

  vk::SamplerCreateInfo sci;
  sci.addressModeU = vk::SamplerAddressMode::eRepeat;
  sci.addressModeV = vk::SamplerAddressMode::eRepeat;
  sci.addressModeW = vk::SamplerAddressMode::eRepeat;
  sci.anisotropyEnable = false;
  sci.maxAnisotropy = 1;
  sci.minFilter = vk::Filter::eLinear;
  sci.magFilter = vk::Filter::eLinear;
  sci.mipmapMode = vk::SamplerMipmapMode::eNearest;
  sci.mipLodBias = 0;
  sci.compareOp = vk::CompareOp::eNever;
  sci.minLod = 0;
  sci.maxLod = 0;
  sci.borderColor = vk::BorderColor::eFloatOpaqueWhite;
  sci.unnormalizedCoordinates = false;

  sampler = vu->device.createSampler(sci);

}

void NeiVu::Texture::setReadOptimal(){
  vu->changeImageLayout(image, vk::ImageLayout::ePreinitialized, vk::ImageLayout::eShaderReadOnlyOptimal, vk::ImageAspectFlagBits::eColor, vk::AccessFlagBits::eHostWrite);
}

void * NeiVu::Texture::map() {
  vk::ImageSubresource subresource;
  subresource.arrayLayer = 0;
  subresource.mipLevel = 0;
  subresource.aspectMask = vk::ImageAspectFlagBits::eColor;
  vk::SubresourceLayout subresourceLayout = vu->device.getImageSubresourceLayout(image, subresource);
  return vu->device.mapMemory(memory, 0, size, vk::MemoryMapFlags());
}

void NeiVu::Texture::unmap() {
  vu->device.unmapMemory(memory);
}
