#include "VuInfo.h"
#include <iostream>
#include <iomanip>

using namespace std;

#define tab setw(o*2)<<""
#define pr tab<<left<<setw(w-o*2)

void NeiVu::printLayers(){
	auto layerProperties = vk::enumerateInstanceLayerProperties();
	cout << "Instance Layers:\n";
	for (auto &l : layerProperties) {
		std::cout << "  " << l.layerName << "\n";
	}
}

void NeiVu::printInstanceExtensions(){
	auto extensionProperties = vk::enumerateInstanceExtensionProperties();
	cout << "Instance Extensions:\n";
	for (auto e : extensionProperties) {
		std::cout << "  " << e.extensionName << "\n";
	}
}

void NeiVu::printDevices(vk::Instance &instance){
	auto dev = instance.enumeratePhysicalDevices();
	for (int i = 0; i < dev.size(); i++) {
		cout << "Device " << i << ":\n";
		printDeviceProperties(dev[i]);
	}
}

void NeiVu::printDeviceProperties(vk::PhysicalDevice &device){
	vk::PhysicalDeviceProperties prop = device.getProperties();
	vk::PhysicalDeviceLimits limits = prop.limits;

	int w = 50;
	int o = 1;

	// properties + limits
	{
		std::cout << pr << "Device name:" << prop.deviceName << "\n";
		std::cout << pr << "Device ID:" << prop.deviceID<< "\n";
		std::cout << pr << "Device type:" << vk::to_string((vk::PhysicalDeviceType)prop.deviceType) << "\n";
		std::cout << pr << "Driver version:" << VK_VERSION_MAJOR(prop.driverVersion) << "."
			<< VK_VERSION_MINOR(prop.driverVersion) << "."
			<< VK_VERSION_PATCH(prop.driverVersion) << "\n";
		std::cout << pr << "API version:" << VK_VERSION_MAJOR(prop.apiVersion) << "."
			<< VK_VERSION_MINOR(prop.apiVersion) << "."
			<< VK_VERSION_PATCH(prop.apiVersion) << "\n";
		std::cout << pr << "Vendor ID:" << prop.vendorID << "\n";
		std::cout << pr << "Device limits:" << "\n";
		o++;
		std::cout << pr << "Buffer image granularity:" << limits.bufferImageGranularity << "\n";
		std::cout << pr << "Discrete queue priorities:" << limits.discreteQueuePriorities << "\n";
		std::cout << pr << "Framebuffer color sample counts:" << vk::to_string(limits.framebufferColorSampleCounts) << "\n";
		std::cout << pr << "Framebuffer depth sample counts:" << vk::to_string(limits.framebufferDepthSampleCounts) << "\n";
		std::cout << pr << "Framebuffer attachment sample counts:" << vk::to_string(limits.framebufferNoAttachmentsSampleCounts) << "\n";
		std::cout << pr << "Framebuffer stencil sample counts:" << vk::to_string(limits.framebufferStencilSampleCounts) << "\n";
		std::cout << pr << "Line width granularity:" << limits.lineWidthGranularity << "\n";
		std::cout << pr << "Line width range:" << limits.lineWidthRange[0] << "-" << limits.lineWidthRange[1] << "\n";
		std::cout << pr << "Max bound descriptor sets:" << limits.maxBoundDescriptorSets << "\n";
		std::cout << pr << "Max clip distances:" << limits.maxClipDistances << "\n";
		std::cout << pr << "..." << "\n";
		o--;
	}


	// family info
	{
		auto qfp = device.getQueueFamilyProperties();
		for (int j = 0; j < qfp.size(); ++j) {
			auto q = qfp.at(j);

			std::cout << tab << "Queue family " << j << ":\n";
			o++;


			std::cout << pr << "Queue flags:" << vk::to_string(q.queueFlags) << endl;
			std::cout << pr << "Queue count:" << q.queueCount << endl;
			std::cout << pr << "Timestamp valid bits:" << q.timestampValidBits << endl;
			vk::Extent3D g = q.minImageTransferGranularity;
			std::cout << pr << "Min image transfer granularity:" << g.width << "," << g.height << "," << g.depth << endl;
			o--;
		}
	}


	// features
	{
		auto features = device.getFeatures();
		std::cout << tab << "Device features:" << endl;
		o++;
		std::cout << pr << "alphaToOne" << features.alphaToOne << endl;
		std::cout << pr << "depthBiasClamp" << features.depthBiasClamp << endl;
		std::cout << pr << "depthClamp" << features.depthClamp << endl;
		std::cout << pr << "drawIndirectFirstInstance" << features.drawIndirectFirstInstance << endl;
		std::cout << pr << "dualSrcBlend" << features.dualSrcBlend << endl;
		std::cout << pr << "fillModeNonSolid" << features.fillModeNonSolid << endl;
		std::cout << pr << "fragmentStoresAndAtomics" << features.fragmentStoresAndAtomics << endl;
		std::cout << pr << "fullDrawIndexUint32" << features.fullDrawIndexUint32 << endl;
		std::cout << pr << "geometryShader" << features.geometryShader << endl;
		std::cout << pr << "imageCubeArray" << features.imageCubeArray << endl;
		std::cout << pr << "independentBlend" << features.independentBlend << endl;
		std::cout << pr << "inheritedQueries" << features.inheritedQueries << endl;
		std::cout << pr << "largePoints" << features.largePoints << endl;
		std::cout << pr << "logicOp" << features.logicOp << endl;
		std::cout << pr << "multiDrawIndirect" << features.multiDrawIndirect << endl;
		std::cout << pr << "multiViewport" << features.multiViewport << endl;
		std::cout << pr << "occlusionQueryPrecise" << features.occlusionQueryPrecise << endl;
		std::cout << pr << "pipelineStatisticsQuery" << features.pipelineStatisticsQuery << endl;
		std::cout << pr << "robustBufferAccess" << features.robustBufferAccess << endl;
		std::cout << pr << "samplerAnisotropy" << features.samplerAnisotropy << endl;
		std::cout << pr << "sampleRateShading" << features.sampleRateShading << endl;
		std::cout << pr << "shaderClipDistance" << features.shaderClipDistance << endl;
		std::cout << pr << "shaderCullDistance" << features.shaderCullDistance << endl;
		std::cout << pr << "shaderFloat64" << features.shaderFloat64<< endl;
		std::cout << pr << "shaderImageGatherExtended" << features.shaderImageGatherExtended << endl;
		std::cout << pr << "shaderInt16" << features.shaderInt16 << endl;
		std::cout << pr << "shaderInt64" << features.shaderInt64 << endl;
		std::cout << pr << "shaderResourceMinLod" << features.shaderResourceMinLod << endl;
		std::cout << pr << "shaderResourceResidency" << features.shaderResourceResidency << endl;
		std::cout << pr << "shaderSampledImageArrayDynamicIndexing" << features.shaderSampledImageArrayDynamicIndexing << endl;
		std::cout << pr << "shaderStorageBufferArrayDynamicIndexing" << features.shaderStorageBufferArrayDynamicIndexing << endl;
		std::cout << pr << "shaderStorageImageArrayDynamicIndexing" << features.shaderStorageImageArrayDynamicIndexing << endl;
		std::cout << pr << "shaderStorageImageExtendedFormats" << features.shaderStorageImageExtendedFormats << endl;
		std::cout << pr << "shaderStorageImageMultisample" << features.shaderStorageImageMultisample << endl;
		std::cout << pr << "shaderStorageImageReadWithoutFormat" << features.shaderStorageImageReadWithoutFormat << endl;
		std::cout << pr << "shaderStorageImageWriteWithoutFormat" << features.shaderStorageImageWriteWithoutFormat << endl;
		std::cout << pr << "shaderTessellationAndGeometryPointSize" << features.shaderTessellationAndGeometryPointSize << endl;
		std::cout << pr << "shaderUniformBufferArrayDynamicIndexing" << features.shaderUniformBufferArrayDynamicIndexing << endl;
		std::cout << pr << "sparseBinding" << features.sparseBinding << endl;
		std::cout << pr << "sparseResidency16Samples" << features.sparseResidency16Samples << endl;
		std::cout << pr << "sparseResidency2Samples" << features.sparseResidency2Samples << endl;
		std::cout << pr << "sparseResidency4Samples" << features.sparseResidency4Samples << endl;
		std::cout << pr << "sparseResidency8Samples" << features.sparseResidency8Samples << endl;
		std::cout << pr << "sparseResidencyAliased" << features.sparseResidencyAliased << endl;
		std::cout << pr << "sparseResidencyBuffer" << features.sparseResidencyBuffer << endl;
		std::cout << pr << "sparseResidencyImage2D" << features.sparseResidencyImage2D << endl;
		std::cout << pr << "sparseResidencyImage3D" << features.sparseResidencyImage3D << endl;
		std::cout << pr << "tessellationShader" << features.tessellationShader << endl;
		std::cout << pr << "textureCompressionASTC_LDR" << features.textureCompressionASTC_LDR << endl;
		std::cout << pr << "textureCompressionBC" << features.textureCompressionBC << endl;
		std::cout << pr << "textureCompressionETC2" << features.textureCompressionETC2 << endl;
		std::cout << pr << "variableMultisampleRate" << features.variableMultisampleRate << endl;
		std::cout << pr << "vertexPipelineStoresAndAtomics" << features.vertexPipelineStoresAndAtomics << endl;
		std::cout << pr << "wideLines" << features.wideLines << endl;
		o--;
	}

	// memory properties
	{
		auto mem = device.getMemoryProperties();
		std::cout << tab << "Memory heaps:" << endl;
		o++;
		for (int i = 0; i < mem.memoryHeapCount; i++) {
			auto m = mem.memoryHeaps[i];
			std::cout << tab << "Heap index:" << i << endl;
			o++;
			std::cout << pr << "Size:" << m.size << " (" << m.size / 1024 / 1024 << "MB)" << endl;
			std::cout << pr << "Flags: " << vk::to_string(m.flags) << endl;
			o--;
		}
		o--;
		std::cout << tab << "Memory types" << endl;
		o++;
		for (int i = 0; i < mem.memoryTypeCount; i++) {
			auto m = mem.memoryTypes[i];
			std::cout << tab << "Memory index:" << i << endl;
			o++;
			std::cout << pr << "Heap index:" << m.heapIndex << endl;
			std::cout << pr << "Flags:" << vk::to_string(m.propertyFlags) << endl;
			o--;
		}
		o--;
	}

	// image format properties
	{
		std::cout << tab << "Image format properties:" << endl;
		o++;
		vk::Format format[] = {
			vk::Format::eR8Uint,
			vk::Format::eR8G8B8A8Uint,
			vk::Format::eR32Sfloat,
			vk::Format::eR32G32B32A32Sfloat,
			vk::Format::eD16Unorm,
			vk::Format::eD16UnormS8Uint,
			vk::Format::eD24UnormS8Uint,
			vk::Format::eD32Sfloat,
			vk::Format::eD32SfloatS8Uint,
		};

		vk::ImageType type[] = {
			vk::ImageType::e1D,
			vk::ImageType::e2D,
			vk::ImageType::e3D
		};

		for (int j = 0; j < 3; j++)
			for (int i = 0; i < 8; i++)
			{
				try {
					auto img = device.getImageFormatProperties(
						format[i],
						type[j],
						vk::ImageTiling::eOptimal,
						vk::ImageUsageFlagBits::eSampled,
						vk::ImageCreateFlags()
					);

					std::cout << tab << vk::to_string(type[j]) << ":" << vk::to_string(format[i]) << endl;
					o++;
					std::cout << pr << "size:" << img.maxExtent.width << "x" << img.maxExtent.height << "x" << img.maxExtent.depth << endl;
					std::cout << pr << "max array layers:" << img.maxArrayLayers << endl;
					std::cout << pr << "max mip levels:" << img.maxMipLevels << endl;
					//std::cout << pr << "max resource size:"<<img.maxResourceSize() << " (" << img.maxResourceSize() / 1024 / 1024 << "MB)" << endl;
					std::cout << pr << "sample counts:" << vk::to_string(img.sampleCounts) << endl;
					o--;
				}
				catch (...) { std::cout << tab << vk::to_string(type[j]) << ":" << vk::to_string(format[i]) << " unsupported" << endl; }
			}
	}

	// device extensions and layers
	{
		auto layers = device.enumerateDeviceLayerProperties();

		auto extensions = device.enumerateDeviceExtensionProperties();

		std::cout << pr << "Device layers:" << endl;
		o++;
		for (auto l : layers) {
			std::cout << pr << l.layerName << endl;
		}
		o--;
		std::cout << pr << "Device extensions:" << endl;
		o++;
		for (auto e : extensions) {
			std::cout << pr << e.extensionName << endl;
		}
		o--;
		
	}
}