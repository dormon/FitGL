#include <VuApp.h>

using namespace NeiVu;


class VuTest : public VuApp {
public:
  VuTest() {}

  vk::DescriptorSetLayout descriptorSetLayout;
  vk::PipelineLayout pipelineLayout;
  vk::Pipeline pipeline;
  vk::PipelineCache pipelineCache;
  std::vector<vk::CommandBuffer> commandBuffers;
  uint32_t currentBuffer = 0;

  void createPipeline() {
    vk::PipelineCacheCreateInfo pipelineCacheInfo;
    pipelineCache = device.createPipelineCache(pipelineCacheInfo);

    vk::PipelineShaderStageCreateInfo shaderStageInfo[2] = {
      vk::PipelineShaderStageCreateInfo()
      .setStage(vk::ShaderStageFlagBits::eVertex)
      .setModule(loadShader(RESOURCE_DIR"shaders/NeiVu/test.vert"))
      .setPName("main"),
      vk::PipelineShaderStageCreateInfo()
      .setStage(vk::ShaderStageFlagBits::eFragment)
      .setModule(loadShader(RESOURCE_DIR"shaders/NeiVu/test.frag"))
      .setPName("main") };

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

    auto const inputAssemblyInfo =
      vk::PipelineInputAssemblyStateCreateInfo().setTopology(
        vk::PrimitiveTopology::eTriangleList);

    auto const viewportInfo = vk::PipelineViewportStateCreateInfo()
      .setViewportCount(1)
      .setScissorCount(1);

    auto const rasterizationInfo =
      vk::PipelineRasterizationStateCreateInfo()
      .setDepthClampEnable(VK_FALSE)
      .setRasterizerDiscardEnable(VK_FALSE)
      .setPolygonMode(vk::PolygonMode::eFill)
      .setCullMode(vk::CullModeFlagBits::eNone)
      .setFrontFace(vk::FrontFace::eCounterClockwise)
      .setDepthBiasEnable(VK_FALSE)
      .setLineWidth(1.0f);

    auto const multisampleInfo = vk::PipelineMultisampleStateCreateInfo();

    auto const stencilOp = vk::StencilOpState()
      .setFailOp(vk::StencilOp::eKeep)
      .setPassOp(vk::StencilOp::eKeep)
      .setCompareOp(vk::CompareOp::eAlways);

    auto const depthStencilInfo =
      vk::PipelineDepthStencilStateCreateInfo()
      .setDepthTestEnable(VK_FALSE)
      .setDepthWriteEnable(VK_TRUE)
      .setDepthCompareOp(vk::CompareOp::eLessOrEqual)
      .setDepthBoundsTestEnable(VK_FALSE)
      .setStencilTestEnable(VK_FALSE)
      .setFront(stencilOp)
      .setBack(stencilOp);

    vk::PipelineColorBlendAttachmentState const colorBlendAttachments[1] = {
      vk::PipelineColorBlendAttachmentState().setColorWriteMask(
        vk::ColorComponentFlagBits::eR |
        vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB |
        vk::ColorComponentFlagBits::eA) };

    auto const colorBlendInfo = vk::PipelineColorBlendStateCreateInfo()
      .setAttachmentCount(1)
      .setPAttachments(colorBlendAttachments);

    vk::DynamicState const dynamicStates[2] = { vk::DynamicState::eViewport,
      vk::DynamicState::eScissor };

    auto const dynamicStateInfo = vk::PipelineDynamicStateCreateInfo()
      .setPDynamicStates(dynamicStates)
      .setDynamicStateCount(2);

    auto const gpci = vk::GraphicsPipelineCreateInfo()
      .setStageCount(2)
      .setPStages(shaderStageInfo)
      .setPVertexInputState(&vertexInputInfo)
      .setPInputAssemblyState(&inputAssemblyInfo)
      .setPViewportState(&viewportInfo)
      .setPRasterizationState(&rasterizationInfo)
      .setPMultisampleState(&multisampleInfo)
      .setPDepthStencilState(&depthStencilInfo)
      .setPColorBlendState(&colorBlendInfo)
      .setPDynamicState(&dynamicStateInfo)
      .setLayout(pipelineLayout)
      .setRenderPass(renderPass);

    pipeline = device.createGraphicsPipeline(pipelineCache, gpci);
  }

  void createDrawCommands() {
    vk::CommandBufferAllocateInfo cbai;
    cbai.commandPool = commandPool;
    cbai.commandBufferCount = swapchainImages.size();

    commandBuffers = device.allocateCommandBuffers(cbai);

    for (int i = 0; i < swapchainImages.size(); i++) {
      auto &commandBuffer = commandBuffers[i];

      auto const commandInfo = vk::CommandBufferBeginInfo().setFlags(
        vk::CommandBufferUsageFlagBits::eSimultaneousUse);

      vk::ClearValue const clearValues[2] = {
        vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f })),
        vk::ClearDepthStencilValue(1.0f, 0u) };

      auto const passInfo =
        vk::RenderPassBeginInfo()
        .setRenderPass(renderPass)
        .setFramebuffer(framebuffers[i])
        .setRenderArea(
          vk::Rect2D(vk::Offset2D(0, 0),
            vk::Extent2D((uint32_t)width, (uint32_t)height)))
        .setClearValueCount(2)
        .setPClearValues(clearValues);

      commandBuffer.begin(commandInfo);

      auto const image_memory_barrier =
        vk::ImageMemoryBarrier()
        .setSrcAccessMask(vk::AccessFlagBits::eMemoryRead)
        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
        .setOldLayout(vk::ImageLayout::ePresentSrcKHR)
        .setNewLayout(vk::ImageLayout::eColorAttachmentOptimal)
        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setImage(swapchainImages[i])
        .setSubresourceRange(vk::ImageSubresourceRange(
          vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

      commandBuffer.pipelineBarrier(
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::DependencyFlagBits(), 0, nullptr, 0, nullptr, 1,
        &image_memory_barrier);

      commandBuffer.beginRenderPass(passInfo, vk::SubpassContents::eInline);

      commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
      commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
        pipelineLayout, 0, 0, 0, 0,
        nullptr);

      auto const viewport = vk::Viewport()
        .setWidth((float)width)
        .setHeight((float)height)
        .setMinDepth((float)0.0f)
        .setMaxDepth((float)1.0f);
      commandBuffer.setViewport(0, 1, &viewport);

      vk::Rect2D const scissor(vk::Offset2D(0, 0),
        vk::Extent2D(width, height));
      commandBuffer.setScissor(0, 1, &scissor);
      commandBuffer.draw(3, 1, 0, 0);
      commandBuffer.endRenderPass();
    }
  }

  virtual void init() {

    vk::DescriptorSetLayoutCreateInfo dslci;
    descriptorSetLayout = device.createDescriptorSetLayout(dslci);
    vk::PipelineLayoutCreateInfo plci;
    plci.setLayoutCount = 1;
    plci.pSetLayouts = &descriptorSetLayout;
    pipelineLayout = device.createPipelineLayout(plci);

    createPipeline();

    flushCommandBuffer();

    createDrawCommands();

  }

  virtual void draw() {
    device.waitIdle();

    vk::Semaphore presentCompleteSemaphore =
      device.createSemaphore(vk::SemaphoreCreateInfo(), 0);

    
    auto i = device.acquireNextImageKHR(swapchain, UINT64_MAX, presentCompleteSemaphore, vk::Fence());
    currentBuffer = i.value;

    /*changeImageLayout(swapchainBuffers[currentBuffer].image,
      vk::ImageLayout::ePresentSrcKHR,
      vk::ImageLayout::eColorAttachmentOptimal,
      vk::ImageAspectFlagBits::eColor);
    flushInitCommandBuffer();*/

    vk::PipelineStageFlags flags = vk::PipelineStageFlagBits::eBottomOfPipe;
    vk::SubmitInfo si;
    si.waitSemaphoreCount=(1);
    si.pWaitSemaphores=(&presentCompleteSemaphore);
    si.pWaitDstStageMask=(&flags);
    si.commandBufferCount=(1);
    si.pCommandBuffers=(&commandBuffers[currentBuffer]);

    queue.submit({si}, vk::Fence());

    vk::PresentInfoKHR pi;
    pi.swapchainCount=(1);
    pi.pSwapchains=(&swapchain);
    pi.pImageIndices=(&currentBuffer);

    queue.presentKHR(pi);

    queue.waitIdle();
    device.destroySemaphore(presentCompleteSemaphore, 0);


    device.waitIdle();

  
  }


};

int main(int /*argc*/, char ** /*argv*/) {
  BaseApp::options.minimumVersion = 0;
  VuTest app;

  return app.run();
}