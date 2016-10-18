#pragma once
#include <BaseApp.h>
#include <vulkan/vulkan.hpp>

namespace NeiVu {
	class VuApp;
};

class NeiVu::VuApp: public BaseApp {
public:
	VuApp();



protected:
	bool debugVulkan = true;
	bool verbose = true;

	vk::Instance instance;
private:
	void createInstance();
	void createDevice();
};
