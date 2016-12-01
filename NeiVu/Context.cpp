#include "Context.h"

#include "VuInfo.h"
#include "VuDebug.h"
#include "Loader.h"

using namespace NeiVu;
using namespace std;

Context* NeiVu::Context::currentContext = nullptr;
NeiVu::Context::Context(ContextOptions & op) :
  options(op) {
  createInstance();
  createDevice();
  createCommandPool();
  createCommandBuffer();
  createPipelineCache();
}

NeiVu::Context::~Context() {
}


vk::ShaderModule NeiVu::Context::loadShader(std::string name){
  auto s = fgl::Loader::binary(name);
  vk::ShaderModuleCreateInfo smci;
  smci.codeSize = s.size();
  smci.pCode = (uint32_t*)s.data();
  return device.createShaderModule(smci, 0);
}

void NeiVu::Context::flushCommandBuffer(){
  commandBuffer.end();
  vk::SubmitInfo si;
  si.commandBufferCount = 1;
  si.pCommandBuffers = &commandBuffer;

  queue.submit(si,0);
  queue.waitIdle();

  //commandBuffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
  commandBuffer.reset(vk::CommandBufferResetFlags());
  commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse));
}

void NeiVu::Context::changeImageLayout(vk::CommandBuffer buffer, vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlagBits imageAspect, vk::AccessFlags srcAccessMask, int mip){
  vk::ImageMemoryBarrier imb;
  imb.oldLayout = oldLayout;
  imb.newLayout = newLayout;
  imb.image = image;
  imb.subresourceRange = vk::ImageSubresourceRange(imageAspect, 0, mip, 0, 1);
  imb.srcAccessMask = srcAccessMask;

  switch (newLayout) {
  case vk::ImageLayout::eTransferDstOptimal:
    imb.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
    break;
  case vk::ImageLayout::eTransferSrcOptimal:
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
  case vk::ImageLayout::ePresentSrcKHR:
    imb.dstAccessMask = vk::AccessFlagBits::eMemoryRead;
    break;
  }

  buffer.pipelineBarrier(
    vk::PipelineStageFlagBits::eTopOfPipe,
    vk::PipelineStageFlagBits::eTopOfPipe,
    vk::DependencyFlags(), 0, NULL, 0, NULL, 1, &imb);
}

void NeiVu::Context::changeImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlagBits imageAspect, vk::AccessFlags srcAccessMask, int mip){
  changeImageLayout(commandBuffer,image,oldLayout, newLayout,imageAspect,srcAccessMask, mip);
}

uint32_t NeiVu::Context::memoryTypeBitsToIndex(uint32_t typeBits, vk::MemoryPropertyFlags requirements){
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

void NeiVu::Context::createInstance() {
  if (options.verbose) {
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

  if (options.debug) {
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

  if (options.debug) {
    enableDebugCallbacks(instance);
  }
}

void NeiVu::Context::createDevice() {
  if (options.verbose)printDevices(instance);

  auto physicalDevices = instance.enumeratePhysicalDevices();
  int deviceCount = physicalDevices.size();

  if (deviceCount > 1) {
    if (options.selectedDevice >= 0 &&
      options.selectedDevice <deviceCount) {
      physicalDevice = physicalDevices[options.selectedDevice];
    }
  }

  if (!physicalDevice) {
    physicalDevice = physicalDevices[0];
  }

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
  dqci.queueCount = options.queueCount;
  dqci.queueFamilyIndex = universalQueueIndex;
  dqci.pQueuePriorities = priorities;

  vector<const char*> enabledLayers;
  if (options.debug && isLayerPresent(physicalDevice, "VK_LAYER_LUNARG_standard_validation"))
    enabledLayers.push_back("VK_LAYER_LUNARG_standard_validation");

  vector<const char*> enabledExtensions;
  enabledExtensions.push_back("VK_KHR_swapchain");
  if(isExtensionPresent(physicalDevice,"VK_NV_glsl_shader"))
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

void NeiVu::Context::createCommandPool(){
  vk::CommandPoolCreateInfo cpci;
  cpci.queueFamilyIndex = universalQueueIndex;
  cpci.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
  commandPool = device.createCommandPool(cpci, 0);
}

void NeiVu::Context::createCommandBuffer(){
  vk::CommandBufferAllocateInfo cbai;
  cbai.commandPool = commandPool;
  cbai.commandBufferCount = 1;
  cbai.level = vk::CommandBufferLevel::ePrimary;

  commandBuffer = device.allocateCommandBuffers(cbai)[0];
  //commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
  commandBuffer.begin(vk::CommandBufferBeginInfo());
}

void NeiVu::Context::createPipelineCache(){
  vk::PipelineCacheCreateInfo pipelineCacheInfo;
  pipelineCache = device.createPipelineCache(pipelineCacheInfo);
}
