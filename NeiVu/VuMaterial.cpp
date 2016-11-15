#include "VuMaterial.h"
#include <iostream>
NeiVu::Material::Material(Context * vu) {
  this->vu = vu;
  if (!vu) this->vu = Context::getCurrent();
}

void NeiVu::Material::bindDescriptor(vk::CommandBuffer cb, vk::PipelineLayout pipelineLayout){
  //std::cout << "Material::bindDescriptor\n";
  cb.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, { set }, {});
}

NeiVu::PhongMaterial::PhongMaterial(Context * vu) :Material(vu) {
}

void NeiVu::PhongMaterial::createDescriptor(vk::DescriptorSetLayout layout){
  std::cout << "PhongMaterial::createDescriptor\n";
  vk::DescriptorPoolSize dps;
  dps.descriptorCount = 1;
  dps.type = vk::DescriptorType::eCombinedImageSampler;

  vk::DescriptorPoolCreateInfo dpci;
  dpci.maxSets = 1;
  dpci.poolSizeCount = 1;
  dpci.pPoolSizes = &dps;

  pool = vu->device.createDescriptorPool(dpci);

  vk::DescriptorSetAllocateInfo dsai;
  dsai.descriptorPool = pool;
  dsai.descriptorSetCount = 1;
  dsai.pSetLayouts = &layout;

  set = vu->device.allocateDescriptorSets(dsai)[0];

  vk::DescriptorImageInfo dii;
  dii.sampler = diffuseTex->sampler;
  dii.imageView = diffuseTex->imageView;
  dii.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

  vk::WriteDescriptorSet wds;
  wds.descriptorCount = 1;
  wds.dstSet = set;
  wds.dstBinding = 1;
  wds.descriptorType = vk::DescriptorType::eCombinedImageSampler;
  wds.pImageInfo = &dii;

  vu->device.updateDescriptorSets({ wds }, {});
}
