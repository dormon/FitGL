#pragma once
#include <vulkan/vulkan.hpp>

namespace NeiVu {
	void printLayers();
	void printInstanceExtensions();
	void printDevices(vk::Instance &instance);
	void printDeviceProperties(vk::PhysicalDevice &device);
	bool isExtensionPresent(std::string name);
	bool isLayerPresent(std::string name);
	bool isExtensionPresent(vk::PhysicalDevice device, std::string name);
	bool isLayerPresent(vk::PhysicalDevice device, std::string name);
};
