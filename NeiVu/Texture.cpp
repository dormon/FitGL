#include "Texture.h"

#include <iostream>

NeiVu::Texture::Texture(Context * vu) :vu(vu) {
  if (vu == nullptr)this->vu = NeiVu::Context::getCurrent();
}

NeiVu::Texture::~Texture() {
}

void NeiVu::Texture::alloc(int width, int height, vk::Format format, int maxMipLevels) {
  std::cout << "Texture::alloc " << width << " " << height << vk::to_string(format) << "\n";
  this->width = width;
  this->height = height;
  this->format = format;

  assert(
    format == vk::Format::eR8G8B8A8Unorm ||
    format == vk::Format::eB8G8R8A8Unorm);

  int mip = 0;
  int size = 0;
  int d = 1;
  for (; mip < maxMipLevels; mip++) {
    int w = width / d; if (width < d)w = 1;
    int h = height / d; if (height < d)h = 1;
    mipOffsets.push_back(size);
    mipWidths.push_back(w);
    mipHeights.push_back(h);
    size += w*h * 4;
    std::cout << "w h s " << w << " " << h << " " << size << "\n";
    d *= 2;
    if (w == 1 && h == 1)break;
  }
  this->mipLevels = mip;
  this->bufferSize = size;

  std::cout << "mip levels " << mip << " size " << size << "\n";

  vk::BufferCreateInfo bci;
  bci.usage = vk::BufferUsageFlagBits::eTransferSrc;
  bci.size = size;

  buffer = vu->device.createBuffer(bci);
  auto mem = vu->device.getBufferMemoryRequirements(buffer);
  int index = vu->memoryTypeBitsToIndex(mem.memoryTypeBits,
    vk::MemoryPropertyFlagBits::eHostVisible |
    vk::MemoryPropertyFlagBits::eHostCoherent);

  vk::MemoryAllocateInfo mai;
  mai.allocationSize = mem.size;
  mai.memoryTypeIndex = index;

  bufferMemory = vu->device.allocateMemory(mai);
  vu->device.bindBufferMemory(buffer, bufferMemory, 0);


  /*
  vk::ImageCreateInfo ici;
  ici.arrayLayers = 1;
  ici.extent = vk::Extent3D(width, height, 1);
  ici.format = format;
  ici.imageType = vk::ImageType::e2D;
  ici.initialLayout = vk::ImageLayout::ePreinitialized;
  ici.mipLevels = mipLevels;
  ici.tiling = vk::ImageTiling::eLinear;
  ici.usage = vk::ImageUsageFlagBits::eTransferSrc;

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
  */
  vk::SamplerCreateInfo sci;
  sci.addressModeU = vk::SamplerAddressMode::eRepeat;
  sci.addressModeV = vk::SamplerAddressMode::eRepeat;
  sci.addressModeW = vk::SamplerAddressMode::eRepeat;
  sci.anisotropyEnable = false;
  sci.maxAnisotropy = 1;
  sci.minFilter = vk::Filter::eLinear;
  sci.magFilter = vk::Filter::eLinear;
  sci.mipmapMode = vk::SamplerMipmapMode::eLinear;
  sci.mipLodBias = 0;
  sci.compareOp = vk::CompareOp::eNever;
  sci.minLod = 0;
  sci.maxLod = mipLevels;
  sci.borderColor = vk::BorderColor::eFloatOpaqueWhite;
  sci.unnormalizedCoordinates = false;

  sampler = vu->device.createSampler(sci);

}
void NeiVu::Texture::generateMipmaps() {
  unsigned char* ptr = (unsigned char*)map();

  for (int i = 1; i < mipLevels; i++) {
    int w = mipWidths[i];
    int h = mipHeights[i];
    int o = mipOffsets[i];

    int o1 = mipOffsets[i-1];
    int w1 = mipWidths[i-1];
    int h1 = mipHeights[i-1];

    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        for (int c = 0; c < 4; c++) {
          int s = 0;
          s += ptr[o1 + ((y*2)*w1 + (x*2)) * 4 + c];
          s += ptr[o1 + ((y*2+1)*w1 + (x*2)) * 4 + c];
          s += ptr[o1 + ((y*2+1)*w1 + (x*2+1)) * 4 + c];
          s += ptr[o1 + ((y*2)*w1 + (x*2+1)) * 4 + c];
          ptr[o + (y*w +x) * 4 + c] = s/4;
        }
      }
    }
  }
  unmap();
}

void * NeiVu::Texture::map() {
  return vu->device.mapMemory(bufferMemory, 0, VK_WHOLE_SIZE, vk::MemoryMapFlags());
}

void NeiVu::Texture::unmap() {
  vu->device.unmapMemory(bufferMemory);
}

void NeiVu::Texture::moveToGPU() {
  generateMipmaps();
  if (!imageGPU) {
    vk::ImageCreateInfo ici;
    ici.arrayLayers = 1;
    ici.extent = vk::Extent3D(width, height, 1);
    ici.format = format;
    ici.imageType = vk::ImageType::e2D;
    ici.initialLayout = vk::ImageLayout::eUndefined;
    ici.mipLevels = mipLevels;
    ici.tiling = vk::ImageTiling::eOptimal;
    ici.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;

    imageGPU = vu->device.createImage(ici);

    auto mem = vu->device.getImageMemoryRequirements(imageGPU);
    size = mem.size;
    int index = vu->memoryTypeBitsToIndex(mem.memoryTypeBits,
      vk::MemoryPropertyFlagBits::eDeviceLocal);

    vk::MemoryAllocateInfo mai;
    mai.allocationSize = mem.size;
    mai.memoryTypeIndex = index;
    memoryGPU = vu->device.allocateMemory(mai);

    vu->device.bindImageMemory(imageGPU, memoryGPU, 0);


    vk::ImageViewCreateInfo ivci;
    ivci.image = imageGPU;
    ivci.viewType = vk::ImageViewType::e2D;
    ivci.format = format;
    ivci.subresourceRange = vk::ImageSubresourceRange(
      vk::ImageAspectFlagBits::eColor, 0, mipLevels, 0, 1);

    imageView = vu->device.createImageView(ivci);
  }

  vu->changeImageLayout(imageGPU, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal,
    vk::ImageAspectFlagBits::eColor,vk::AccessFlags(),mipLevels);

  /*vu->commandBuffer.copyImage(image, vk::ImageLayout::eTransferSrcOptimal,
    imageGPU, vk::ImageLayout::eTransferDstOptimal,
    { vk::ImageCopy(
      vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor,0,0,1),vk::Offset3D(0,0,0),
      vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor,0,0,1),vk::Offset3D(0,0,0),
      vk::Extent3D(width,height,1))});*/
  for (int i = 0; i < mipLevels; i++)
    vu->commandBuffer.copyBufferToImage(buffer, imageGPU, vk::ImageLayout::eTransferDstOptimal,
    { vk::BufferImageCopy(mipOffsets[i],0,0,
      vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor,i,0,1),
      vk::Offset3D(0,0,0),vk::Extent3D(mipWidths[i],mipHeights[i],1)),
    });

  vu->changeImageLayout(imageGPU, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal,
    vk::ImageAspectFlagBits::eColor, vk::AccessFlagBits::eTransferWrite, mipLevels);


}
