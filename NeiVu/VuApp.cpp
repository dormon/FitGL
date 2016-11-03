#include "VuApp.h"
#include "VuInfo.h"
#include "VuDebug.h"
#include <vector>
#include <algorithm>

#include <SDL2/SDL_syswm.h>

using namespace NeiVu;
using namespace std;

VuApp::VuApp() {
  width = mainWindow->getWidth();
  height = mainWindow->getHeight();

  try {
    createInstance();
    createDevice();
    createCommands();
    createSurface();
    createSwapchain();
    createDepthBuffer();
    createRenderPass();
    createFramebuffers();

  }
  catch (system_error er) {
    cout << "error " << er.what() << "\n";
  }
}



vk::ShaderModule NeiVu::VuApp::loadShader(std::string name) {
  auto s = Loader::text(name);
  vk::ShaderModuleCreateInfo smci;
  smci.codeSize = s.size();
  smci.pCode = (uint32_t*)s.data();
  return device.createShaderModule(smci, 0);
}

void VuApp::createInstance() {
  if (verbose) {
    printInstanceExtensions();
    printLayers();
  }
  vector<const char*> enabledExtensions;
  vector<const char*> enabledLayers;

  enabledExtensions.push_back("VK_KHR_surface");
#ifdef WIN32
  enabledExtensions.push_back("VK_KHR_win32_surface");
#endif
  //@todo linux

  if (debugVulkan) {
    auto la = vk::enumerateInstanceLayerProperties();
    auto ex = vk::enumerateInstanceExtensionProperties();

    auto names = vector<string>();
    for (auto &l : la)names.push_back(l.layerName);
    for (auto &e : ex)names.push_back(e.extensionName);

    if (isLayerPresent("VK_LAYER_LUNARG_standard_validation")) {
      enabledLayers.push_back("VK_LAYER_LUNARG_standard_validation");
    }

    if (isExtensionPresent("VK_EXT_debug_report")) {
      enabledExtensions.push_back("VK_EXT_debug_report");
    }
  }

  auto ici = vk::InstanceCreateInfo();
  ici.enabledExtensionCount = (unsigned int)enabledExtensions.size();
  ici.ppEnabledExtensionNames = enabledExtensions.data();
  ici.enabledLayerCount = (unsigned int)enabledLayers.size();
  ici.ppEnabledLayerNames = enabledLayers.data();


  instance = vk::createInstance(ici);
  assert(instance);

  if (debugVulkan) {
    enableDebugCallbacks(instance);
  }

}

void NeiVu::VuApp::createDevice() {
  if (verbose)printDevices(instance);

  auto physicalDevices = instance.enumeratePhysicalDevices();
  if (physicalDevices.size() > 1) cout << "Multiple vulkan devices not supported! Selected first one.";

  physicalDevice = physicalDevices[0];
  auto queueProps = physicalDevice.getQueueFamilyProperties();

  for (int i = 0; i < queueProps.size(); i++) {
    auto &q = queueProps[i];
    if (q.queueFlags & (vk::QueueFlagBits::eTransfer | vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute)) {
      universalQueueIndex = i;
      break;
    }
  }
  assert(universalQueueIndex >= 0 && "Universal queue not found.");

  float priorities[1] = { 1.0 };
  vk::DeviceQueueCreateInfo dqci;
  dqci.queueCount = 1;
  dqci.queueFamilyIndex = universalQueueIndex;
  dqci.pQueuePriorities = { priorities };

  vector<const char*> enabledLayers;
  if (debugVulkan && isLayerPresent(physicalDevice, "VK_LAYER_LUNARG_standard_validation"))
    enabledLayers.push_back("VK_LAYER_LUNARG_standard_validation");

  vector<const char*> enabledExtensions;
  enabledExtensions.push_back("VK_KHR_swapchain");
  enabledExtensions.push_back("VK_NV_glsl_shader");

  vk::DeviceCreateInfo dci;
  dci.queueCreateInfoCount = 1;
  dci.pQueueCreateInfos = &dqci;
  dci.enabledExtensionCount = (unsigned int)enabledExtensions.size();
  dci.ppEnabledExtensionNames = enabledExtensions.data();
  dci.enabledLayerCount = (unsigned int)enabledLayers.size();
  dci.ppEnabledLayerNames = enabledLayers.data();

  device = physicalDevice.createDevice(dci);
  assert(device);

  queue = device.getQueue(universalQueueIndex, 0);
}

void NeiVu::VuApp::createCommands() {
  vk::CommandPoolCreateInfo cpci;
  cpci.queueFamilyIndex = universalQueueIndex;
  commandPool = device.createCommandPool(cpci, 0);

  vk::CommandBufferAllocateInfo cbai;
  cbai.commandPool = commandPool;
  cbai.commandBufferCount = 1;

  commandBuffer = device.allocateCommandBuffers(cbai)[0];
  commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
}

void NeiVu::VuApp::createSurface() {
#ifdef WIN32
  vk::Win32SurfaceCreateInfoKHR wsci;
  wsci.hinstance = (HINSTANCE)GetModuleHandle(NULL);
  SDL_SysWMinfo info;
  SDL_VERSION(&info.version);
  SDL_GetWindowWMInfo(mainWindow->getWindowHandle(), &info);
  wsci.hwnd = (HWND)info.info.win.window;

  surface = instance.createWin32SurfaceKHR(wsci);
  assert(surface);
  auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);
  surfaceFormat = surfaceFormats[0];

  assert(physicalDevice.getSurfaceSupportKHR(universalQueueIndex, surface));

#else
  assert("No pinguins" && 0);
#endif
}

void NeiVu::VuApp::createSwapchain() {
  auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
  auto presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
  if (verbose) {
    std::cout << "surfaceCapabilities:\n";
    std::cout << "  currentExtent " << surfaceCapabilities.currentExtent.height << "," << surfaceCapabilities.currentExtent.width << endl;
    std::cout << "  minImageCount " << surfaceCapabilities.minImageCount << endl;
    std::cout << "  maxImageCount " << surfaceCapabilities.maxImageCount << endl;
    std::cout << "  supportedTransforms " << vk::to_string(surfaceCapabilities.supportedTransforms) << endl;
    std::cout << "  currentTransform " << vk::to_string(surfaceCapabilities.currentTransform) << endl;

    std::cout << "present modes:\n";
    for (auto pm : presentModes) {
      std::cout << " " << vk::to_string(pm) << endl;
    }
  }

  vk::PresentModeKHR presentMode;
  if (find(presentModes.begin(), presentModes.end(), vk::PresentModeKHR::eFifoRelaxed) != presentModes.end()) {
    presentMode = vk::PresentModeKHR::eFifoRelaxed;
  }
  else if (find(presentModes.begin(), presentModes.end(), vk::PresentModeKHR::eFifo) != presentModes.end()) {
    presentMode = vk::PresentModeKHR::eFifo;
  }
  else if (find(presentModes.begin(), presentModes.end(), vk::PresentModeKHR::eImmediate) != presentModes.end()) {
    presentMode = vk::PresentModeKHR::eImmediate;
  }
  else if (find(presentModes.begin(), presentModes.end(), vk::PresentModeKHR::eMailbox) != presentModes.end()) {
    presentMode = vk::PresentModeKHR::eMailbox;
  }

  unsigned int imageCount = 2;
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
  sci.oldSwapchain = NULL;
  sci.pQueueFamilyIndices = NULL;
  sci.presentMode = presentMode;
  sci.preTransform = transform;
  sci.queueFamilyIndexCount = 0;
  sci.surface = surface;

  swapchain = device.createSwapchainKHR(sci);

  swapchainImages = device.getSwapchainImagesKHR(swapchain);

  for (auto &i : swapchainImages) {
    vk::ImageViewCreateInfo ivci;
    ivci.format = surfaceFormat.format;
    ivci.image = i;
    ivci.subresourceRange = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    ivci.viewType = vk::ImageViewType::e2D;
    changeImageLayout(i, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR, vk::ImageAspectFlagBits::eColor, vk::AccessFlags());
    swapchainImageViews.emplace_back(device.createImageView(ivci));
  }
}

void NeiVu::VuApp::createDepthBuffer() {
  depthBuffer.format = vk::Format::eD24UnormS8Uint;

  vk::ImageCreateInfo ici;
  ici.arrayLayers = 1;
  ici.extent = vk::Extent3D(width, height, 1);
  ici.format = depthBuffer.format;
  ici.imageType = vk::ImageType::e2D;
  ici.mipLevels = 1;
  ici.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;

  depthBuffer.image = device.createImage(ici, 0);

  vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(depthBuffer.image);
  uint32_t memoryTypeIndex = memoryTypeBitsToIndex(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlags());

  vk::MemoryAllocateInfo mai;
  mai.allocationSize = memoryRequirements.size;
  mai.memoryTypeIndex = memoryTypeIndex;

  depthBuffer.memory = device.allocateMemory(mai, 0);

  device.bindImageMemory(depthBuffer.image, depthBuffer.memory, 0);

  changeImageLayout(depthBuffer.image, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::ImageAspectFlagBits::eDepth);

  vk::ImageViewCreateInfo ivci;
  ivci.format = depthBuffer.format;
  ivci.image = depthBuffer.image;
  ivci.subresourceRange = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1);
  ivci.viewType = vk::ImageViewType::e2D;

  depthBuffer.view = device.createImageView(ivci);
}

void NeiVu::VuApp::createRenderPass() {
  vk::AttachmentDescription attachments[2];
  attachments[0].format = surfaceFormat.format;
  attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
  attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
  attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  attachments[0].initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
  attachments[0].finalLayout = vk::ImageLayout::ePresentSrcKHR;

  attachments[1].format = depthBuffer.format;
  attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
  attachments[1].storeOp = vk::AttachmentStoreOp::eDontCare;
  attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  attachments[1].initialLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
  attachments[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

  vk::AttachmentReference colorReference(0, vk::ImageLayout::eColorAttachmentOptimal);
  vk::AttachmentReference depthrReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

  vk::SubpassDescription subpass;
  subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorReference;
  subpass.pDepthStencilAttachment = &depthrReference;

  vk::RenderPassCreateInfo rpci;
  rpci.attachmentCount = 2;
  rpci.pAttachments = attachments;
  rpci.subpassCount = 1;
  rpci.pSubpasses = &subpass;

  renderPass = device.createRenderPass(rpci, 0);
}

void NeiVu::VuApp::createFramebuffers() {
  vk::ImageView attachments[2];
  attachments[1] = depthBuffer.view;

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

void NeiVu::VuApp::flushCommandBuffer(){
  commandBuffer.end();
  vk::SubmitInfo si;
  si.commandBufferCount = 1;
  si.pCommandBuffers = &commandBuffer;

  queue.submit({ si },vk::Fence());
  queue.waitIdle();

  commandBuffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
  vk::CommandBufferAllocateInfo cbai;
  cbai.commandPool = commandPool;
  cbai.commandBufferCount = 1;

  commandBuffer = device.allocateCommandBuffers(cbai)[0];
  commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
}

uint32_t NeiVu::VuApp::memoryTypeBitsToIndex(uint32_t typeBits, vk::MemoryPropertyFlags requirements) {
  vk::PhysicalDeviceMemoryProperties prop = physicalDevice.getMemoryProperties();
  auto mt = prop.memoryTypes;

  for (int i = 0; i < prop.memoryTypeCount; i++) {
    if ((typeBits & 1) && ((mt[i].propertyFlags&requirements) == requirements)) {
      return i;
    }
    typeBits >>= 1;
  }
  return -1;
}

void NeiVu::VuApp::changeImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlagBits imageAspect, vk::AccessFlags srcAccessMask) {

  vk::ImageMemoryBarrier imb;
  imb.oldLayout = oldLayout;
  imb.newLayout = newLayout;
  imb.image = image;
  imb.subresourceRange = vk::ImageSubresourceRange(imageAspect, 0, 1, 0, 1);
  imb.srcAccessMask = srcAccessMask;

  switch (newLayout) {
  case vk::ImageLayout::eTransferDstOptimal:
    imb.dstAccessMask = vk::AccessFlagBits::eTransferRead;
    break;
  case vk::ImageLayout::eColorAttachmentOptimal:
    imb.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    break;
  case vk::ImageLayout::eDepthStencilAttachmentOptimal:
    imb.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    break;
  case vk::ImageLayout::eShaderReadOnlyOptimal:
    imb.dstAccessMask = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eInputAttachmentRead;
    break;
  }

  commandBuffer.pipelineBarrier(
    vk::PipelineStageFlagBits::eTopOfPipe,
    vk::PipelineStageFlagBits::eTopOfPipe,
    vk::DependencyFlags(), 0, NULL, 0, NULL, 1, &imb);

}