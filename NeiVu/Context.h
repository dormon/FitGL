#pragma once
namespace NeiVu {
  class Context;
  class ContextOptions;
};

#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.hpp>


class NeiVu::ContextOptions {
public:
  bool debug = true;
  bool verbose = true;
  int selectedDevice = -1;
  int queueCount = 1;
};

class NeiVu::Context {
public:

  Context(ContextOptions &op = ContextOptions());
  ~Context();

  static void makeCurrent(Context* c) { currentContext = c; }
  static Context* getCurrent() { return currentContext; }

  ContextOptions options;
  vk::Instance instance;
  vk::PhysicalDevice physicalDevice;
  vk::Device device;
  
  vk::Queue queue;
  vk::CommandPool commandPool;
  vk::CommandBuffer commandBuffer;

  vk::PipelineCache pipelineCache;

  int universalQueueIndex = -1;


  vk::ShaderModule loadShader(std::string name);
  
  void flushCommandBuffer();
  void changeImageLayout(vk::CommandBuffer buffer, vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlagBits imageAspect, vk::AccessFlags srcAccessMask = vk::AccessFlags());
  void changeImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlagBits imageAspect, vk::AccessFlags srcAccessMask = vk::AccessFlags());
  uint32_t memoryTypeBitsToIndex(uint32_t typeBits, vk::MemoryPropertyFlags requirements);
private:
  static Context* currentContext;
  void createInstance();
  void createDevice();
  void createCommandPool();
  void createCommandBuffer();
  void createPipelineCache();
};