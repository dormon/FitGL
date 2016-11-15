#pragma once
#include <memory>
namespace NeiVu {
  class Material;
  class PhongMaterial;
  typedef std::shared_ptr<Material> MaterialShared;
  typedef std::shared_ptr<PhongMaterial> PhongMaterialShared;
};
#include <glm/glm.hpp>
#include "Context.h"
#include "Texture.h"

class NeiVu::Material{
public:
  Material(Context* vu = nullptr);
  virtual void createDescriptor(vk::DescriptorSetLayout layout){ }
  virtual void bindDescriptor(vk::CommandBuffer cb, vk::PipelineLayout pipelineLayout);

protected:
  vk::DescriptorPool pool;
  vk::DescriptorSet set;
  Context* vu;
};



class NeiVu::PhongMaterial: public NeiVu::Material{
public:
  PhongMaterial(Context* vu = nullptr);

	glm::vec4 diffuse = glm::vec4(0, 0, 0, 0);
	NeiVu::Texture* diffuseTex;

  virtual void createDescriptor(vk::DescriptorSetLayout layout);
};
