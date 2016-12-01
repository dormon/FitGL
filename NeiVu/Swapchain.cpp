#include "Swapchain.h"
#include "VuInfo.h"

#include <SDL2/SDL_syswm.h>
#include <algorithm>

using namespace std;

NeiVu::Swapchain::Swapchain(SwapchainOptions & op):
options(op){
  instance = op.vu->instance;
  physicalDevice = op.vu->physicalDevice;
  device = op.vu->device;
  vu = op.vu;
  window = op.window;
  imageCount = op.imageCount;

  createSurface();
  createSwapchain();
  createDepthBuffer();
  createRenderPass();
  createFramebuffers();

}

NeiVu::Swapchain::~Swapchain(){
}

uint32_t NeiVu::Swapchain::next(){
  semaphore = device.createSemaphore(vk::SemaphoreCreateInfo());
  auto i = vu->device.acquireNextImageKHR(swapchain, UINT64_MAX, semaphore, 0);
  currentImage = i.value;

  return currentImage;
}

void NeiVu::Swapchain::swap(){
  vk::PresentInfoKHR pi;
  pi.swapchainCount = (1);
  pi.pSwapchains = &swapchain;
  pi.pImageIndices = &currentImage;
  vu->queue.presentKHR(pi);

  vu->queue.waitIdle();
  vu->device.destroySemaphore(semaphore);
}

void NeiVu::Swapchain::resize(int w, int h){
}

vk::Framebuffer NeiVu::Swapchain::currenFramebuffer(){
  return framebuffers[currentImage];
}

vk::Image NeiVu::Swapchain::currenImage(){
  return swapchainImages[currentImage];
}

void NeiVu::Swapchain::createSurface(){
#ifdef WIN32
  vk::Win32SurfaceCreateInfoKHR wsci;
  wsci.hinstance = (HINSTANCE)GetModuleHandle(NULL);
  SDL_SysWMinfo info;
  SDL_VERSION(&info.version);
  SDL_GetWindowWMInfo(window, &info);
  wsci.hwnd = (HWND)info.info.win.window;

  surface = instance.createWin32SurfaceKHR(wsci);
  assert(surface);
  auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);
  surfaceFormat = surfaceFormats[0];

  assert(physicalDevice.getSurfaceSupportKHR(vu->universalQueueIndex, surface));

#else
  //@TODO
  assert("No pinguins" && 0);
#endif
}

void NeiVu::Swapchain::createSwapchain(){
  auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
  auto presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
  if (vu->options.verbose) {
    printSurfaceCapabilities(physicalDevice, surface);
  }

  vk::PresentModeKHR presentMode;

  auto isModePresent = [&](vk::PresentModeKHR mode) {
    return find(presentModes.begin(), presentModes.end(), mode) != presentModes.end();
  };

  std::vector<vk::PresentModeKHR> modes = {
    vk::PresentModeKHR::eImmediate,
    vk::PresentModeKHR::eMailbox,
    //vk::PresentModeKHR::eFifo,
    //vk::PresentModeKHR::eFifoRelaxed
  };
  for (auto &mode : modes) {
    if (isModePresent(mode)) presentMode = mode;
  }

  imageCount = (std::min)(imageCount, surfaceCapabilities.maxImageCount);
  imageCount = (std::max)(imageCount, surfaceCapabilities.minImageCount);

  auto transform = surfaceCapabilities.currentTransform;

  vk::SwapchainCreateInfoKHR sci;
  sci.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  sci.imageArrayLayers = 1;
  sci.imageColorSpace = surfaceFormat.colorSpace;
  sci.imageExtent = surfaceCapabilities.currentExtent;
  sci.imageFormat = surfaceFormat.format;
  sci.imageSharingMode = vk::SharingMode::eExclusive;
  sci.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
  sci.minImageCount = imageCount;
  sci.oldSwapchain = swapchain;
  sci.pQueueFamilyIndices = NULL;
  sci.presentMode = presentMode;
  sci.preTransform = transform;
  sci.queueFamilyIndexCount = 0;
  sci.surface = surface;

  swapchain = device.createSwapchainKHR(sci);

  swapchainImages = device.getSwapchainImagesKHR(swapchain);

  width = surfaceCapabilities.currentExtent.width;
  height = surfaceCapabilities.currentExtent.height;

  for (auto &img : swapchainImages) {
    vk::ImageViewCreateInfo ivci;
    ivci.format = surfaceFormat.format;
    ivci.image = img;
    ivci.subresourceRange = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    ivci.viewType = vk::ImageViewType::e2D;
    vu->changeImageLayout(img, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR, vk::ImageAspectFlagBits::eColor, vk::AccessFlags());
    swapchainImageViews.emplace_back(device.createImageView(ivci));
  }
}

void NeiVu::Swapchain::createDepthBuffer(){
  if(options.floatDepthBuffer && options.stencilBuffer)
    depthBufferFormat = vk::Format::eD32SfloatS8Uint;
  if (options.floatDepthBuffer && !options.stencilBuffer)
    depthBufferFormat = vk::Format::eD32Sfloat;
  if (!options.floatDepthBuffer && options.stencilBuffer)
    depthBufferFormat = vk::Format::eD24UnormS8Uint;
  if (!options.floatDepthBuffer && !options.stencilBuffer)
    depthBufferFormat = vk::Format::eD16Unorm;

  vk::ImageCreateInfo ici;
  ici.arrayLayers = 1;
  ici.extent = vk::Extent3D(width, height, 1);
  ici.format = depthBufferFormat;
  ici.imageType = vk::ImageType::e2D;
  ici.mipLevels = 1;
  ici.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
  ici.tiling = vk::ImageTiling::eOptimal;
  ici.initialLayout = vk::ImageLayout::eUndefined;


  depthBufferImage = device.createImage(ici, 0);

  vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(depthBufferImage);
  uint32_t memoryTypeIndex = vu->memoryTypeBitsToIndex(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

  vk::MemoryAllocateInfo mai;
  mai.allocationSize = memoryRequirements.size;
  mai.memoryTypeIndex = memoryTypeIndex;

  depthBufferMemory = device.allocateMemory(mai, 0);

  device.bindImageMemory(depthBufferImage, depthBufferMemory, 0);

  vu->changeImageLayout(depthBufferImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::ImageAspectFlagBits::eDepth);

  vk::ImageViewCreateInfo ivci;
  ivci.format = depthBufferFormat;
  ivci.image = depthBufferImage;
  ivci.subresourceRange = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1);
  ivci.viewType = vk::ImageViewType::e2D;

  depthBufferView = device.createImageView(ivci);
}

void NeiVu::Swapchain::createRenderPass(){
  vk::AttachmentDescription attachments[2];
  attachments[0].format = surfaceFormat.format;
  attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
  attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
  attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  attachments[0].initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
  attachments[0].finalLayout = vk::ImageLayout::ePresentSrcKHR;

  attachments[1].format = depthBufferFormat;
  attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
  attachments[1].storeOp = vk::AttachmentStoreOp::eDontCare;
  attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eClear;
  attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  attachments[1].initialLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
  attachments[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

  vk::AttachmentReference colorReference(0, vk::ImageLayout::eColorAttachmentOptimal);
  vk::AttachmentReference depthReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

  vk::SubpassDescription subpass;
  subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorReference;
  subpass.pDepthStencilAttachment = &depthReference;

  vk::RenderPassCreateInfo rpci;
  rpci.attachmentCount = 2;
  rpci.pAttachments = attachments;
  rpci.subpassCount = 1;
  rpci.pSubpasses = &subpass;

  renderPass = device.createRenderPass(rpci, 0);
}

void NeiVu::Swapchain::createFramebuffers(){
  vk::ImageView attachments[2];
  attachments[1] = depthBufferView;

  vk::FramebufferCreateInfo fbci;
  fbci.renderPass = renderPass;
  fbci.attachmentCount = 2;
  fbci.pAttachments = attachments;
  fbci.width = width;
  fbci.height = height;
  fbci.layers = 1;

  for (int i = 0; i < swapchainImages.size(); i++) {
    attachments[0] = swapchainImageViews[i];
    framebuffers.emplace_back(device.createFramebuffer(fbci));
  }
}
