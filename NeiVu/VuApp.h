#pragma once
#include <BaseApp.h>


#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.hpp>


class DepthBuffer {
public:
  vk::Format format;
  vk::Image image;
  vk::ImageView view;
  vk::DeviceMemory memory;
};

namespace NeiVu {
	class VuApp;
};

class NeiVu::VuApp: public BaseApp {
public:
	VuApp();

protected:
  int width, height;
	bool debugVulkan = true;
	bool verbose = true;

	int universalQueueIndex = -1;

	vk::Instance instance;
	vk::PhysicalDevice physicalDevice;
	vk::Device device;
	vk::Queue queue;
	vk::CommandPool commandPool;
	vk::SurfaceKHR surface;
	vk::SurfaceFormatKHR surfaceFormat;
	vk::SwapchainKHR swapchain;
  vk::RenderPass renderPass;
	std::vector<vk::Image> swapchainImages;
  std::vector<vk::ImageView> swapchainImageViews;
  std::vector<vk::Framebuffer> framebuffers;

	vk::CommandBuffer commandBuffer;
  DepthBuffer depthBuffer;

  vk::ShaderModule loadShader(std::string name);
private:
	void createInstance();
	void createDevice();
	void createCommands();
	void createSurface();
	void createSwapchain();
  void createDepthBuffer();
  void createRenderPass();
  void createFramebuffers();
protected:
  void flushCommandBuffer();
  uint32_t memoryTypeBitsToIndex(uint32_t typeBits, vk::MemoryPropertyFlags requirements);
  void changeImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlagBits imageAspect, vk::AccessFlags srcAccessMask = vk::AccessFlags());
};
