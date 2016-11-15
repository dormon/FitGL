#pragma once
namespace NeiVu {
  class Swapchain;
  class SwapchainOptions;
};

#include "Context.h"
#include "SDL2/SDL.h"
#include <vector>


class NeiVu::SwapchainOptions {
public:
  NeiVu::Context* vu;
  SDL_Window* window;
  bool imageCount = 2;
  bool floatDepthBuffer = true;
  bool stencilBuffer = false;
};


class NeiVu::Swapchain {
public:
  Swapchain(SwapchainOptions &op = SwapchainOptions());
  ~Swapchain();
  
  vk::SurfaceKHR surface;
  vk::SurfaceFormatKHR surfaceFormat;
  vk::SwapchainKHR swapchain;
  std::vector<vk::Image> swapchainImages;
  std::vector<vk::ImageView> swapchainImageViews;

  vk::Image depthBufferImage;
  vk::ImageView depthBufferView;
  vk::Format depthBufferFormat;
  vk::DeviceMemory depthBufferMemory;

  vk::RenderPass renderPass;
  std::vector<vk::Framebuffer> framebuffers;

  uint32_t next();
  void swap();
  void resize(int w, int h);
  vk::Framebuffer currenFramebuffer();
  vk::Image currenImage();

  uint32_t width;
  uint32_t height;
  uint32_t currentImage;

  vk::Semaphore semaphore;
private:
  SwapchainOptions options;
  unsigned int imageCount;
  vk::Instance instance;
  vk::PhysicalDevice physicalDevice;
  vk::Device device;
  Context* vu;
  SDL_Window* window;



  void createSurface();
  void createSwapchain();
  void createDepthBuffer();
  void createRenderPass();
  void createFramebuffers();
};