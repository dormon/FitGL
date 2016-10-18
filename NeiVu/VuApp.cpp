#include "VuApp.h"
#include "VuInfo.h"
#include "VuDebug.h"
#include <vector>

using namespace NeiVu;
using namespace std;

VuApp::VuApp() {

	createInstance();
	createDevice();
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
		enabledExtensions.push_back("VK_EXT_debug_report");
		enabledLayers.push_back("VK_LAYER_LUNARG_standard_validation");
	}

	auto ici = vk::InstanceCreateInfo();
	ici.enabledExtensionCount = enabledExtensions.size();
	ici.ppEnabledExtensionNames = enabledExtensions.data();
	ici.enabledLayerCount = enabledLayers.size();
	ici.ppEnabledLayerNames = enabledLayers.data();


	instance = vk::createInstance(ici);

	if (debugVulkan) {
		enableDebugCallbacks(instance);
	}
}

void NeiVu::VuApp::createDevice(){
	if (verbose)printDevices(instance);
}
