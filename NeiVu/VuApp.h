#pragma once
#include <BaseApp.h>


#include <vulkan/vulkan.hpp>

#include "Context.h"
#include "Swapchain.h"
#include "Pipeline.h"
#include "Buffer.h"
#include "VuMesh.h"
#include "LoaderAssimp.h"

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

  NeiVu::Context *vu;
  NeiVu::Swapchain *swapchain;
};
