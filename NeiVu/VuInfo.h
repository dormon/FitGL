#pragma once
#include "Context.h"

namespace NeiVu {
	void printLayers();
	void printInstanceExtensions();
	void printDevices(vk::Instance &instance);
	void printDeviceProperties(vk::PhysicalDevice &device);
	bool isExtensionPresent(std::string name);
	bool isLayerPresent(std::string name);
	bool isExtensionPresent(vk::PhysicalDevice device, std::string name);
	bool isLayerPresent(vk::PhysicalDevice device, std::string name);
  void printSurfaceCapabilities(vk::PhysicalDevice device,vk::SurfaceKHR surface);
};
