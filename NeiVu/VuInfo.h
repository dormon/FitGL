#pragma once
#include <vulkan/vulkan.hpp>

namespace NeiVu {
	void printLayers();
	void printInstanceExtensions();
	void printDevices(vk::Instance &instance);
	void printDeviceProperties(vk::PhysicalDevice &device);
};
