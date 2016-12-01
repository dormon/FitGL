#include <VuApp.h>

using namespace NeiVu;
using namespace glm;

class VuTest : public VuApp {
public:
  VuTest(){
    cam = fgl::newPerspectiveCamera();
    manipulator = fgl::newOrbitManipulator(cam);
    manipulator->setupCallbacks(*this);
  }

  vk::DescriptorSetLayout descriptorSetLayout;
  vk::PipelineLayout pipelineLayout;
  vk::CommandBuffer commandBuffer;

  Pipeline* pipeline;

  fgl::PerspectiveCameraS cam;
  fgl::OrbitManipulatorS manipulator;
  NeiVu::NodeShared root;

  vk::QueryPool queryPool;

  int frame = 0;
  float fps = 0;

  void createDescriptors(NeiVu::NodeShared const&node) {
    for (auto &m : node->meshes) {
      m->mat->createDescriptor(descriptorSetLayout);
    }
    for (auto &c : node->children) {
      createDescriptors(c);
    }
  }

  void drawNode(NeiVu::NodeShared const&node, glm::mat4 parentMat) {
    glm::mat4 useMat = parentMat * node->getMatrix();
    commandBuffer.pushConstants<mat4>(pipelineLayout, vk::ShaderStageFlagBits::eVertex, sizeof(mat4) * 2, { useMat });

    glm::mat3 n = transpose(inverse(glm::mat3(useMat)));
    //program.setMatrix3fv("n", value_ptr(n));


    for (auto &m : node->meshes) {
      m->mat->bindDescriptor(commandBuffer, pipelineLayout);
      m->draw(commandBuffer);
    }
    for (auto &c : node->children) {
      drawNode(c, useMat);
    }
  };

  void createPipeline() {
    pipeline = new Pipeline(vu);

    pipeline->addStage(vk::ShaderStageFlagBits::eVertex,
      vu->loadShader(RESOURCE_DIR"shaders/Nei/n00_Test/test.vert.spv"));
    pipeline->addStage(vk::ShaderStageFlagBits::eFragment,
      vu->loadShader(RESOURCE_DIR"shaders/Nei/n00_Test/test.frag.spv"));
    pipeline->renderPass = swapchain->renderPass;
    pipeline->layout = pipelineLayout;
    pipeline->addAttribute(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos));
    pipeline->addAttribute(0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, norm));
    pipeline->addAttribute(0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, tc));
    pipeline->addVertexBuffer(0, sizeof(Vertex));

    pipeline->create();
  }

  void createDrawCommand() {
    commandBuffer.reset(vk::CommandBufferResetFlags());

    auto const commandInfo = vk::CommandBufferBeginInfo().setFlags(
      vk::CommandBufferUsageFlagBits::eSimultaneousUse);
    commandBuffer.begin(commandInfo);

    commandBuffer.resetQueryPool(queryPool, 0, 2);
    commandBuffer.writeTimestamp(vk::PipelineStageFlagBits::eTopOfPipe, queryPool, 0);

    vk::ClearValue const clearValues[2] = {
      vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f })),
      vk::ClearDepthStencilValue(1.0f, 0u) };

    auto const passInfo =
      vk::RenderPassBeginInfo()
      .setRenderPass(swapchain->renderPass)
      .setFramebuffer(swapchain->currenFramebuffer())
      .setRenderArea(
        vk::Rect2D(vk::Offset2D(0, 0),
          vk::Extent2D((uint32_t)width, (uint32_t)height)))
      .setClearValueCount(2)
      .setPClearValues(clearValues);



    vu->changeImageLayout(commandBuffer, swapchain->currenImage(), vk::ImageLayout::ePresentSrcKHR,
      vk::ImageLayout::eColorAttachmentOptimal, vk::ImageAspectFlagBits::eColor, vk::AccessFlagBits::eMemoryRead);

    commandBuffer.beginRenderPass(passInfo, vk::SubpassContents::eInline);


    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->pipeline);

    auto const viewport = vk::Viewport()
      .setWidth((float)width)
      .setHeight((float)height)
      .setMinDepth((float)0.0f)
      .setMaxDepth((float)1.0f);
    commandBuffer.setViewport(0, 1, &viewport);

    vk::Rect2D const scissor(vk::Offset2D(0, 0),
      vk::Extent2D(width, height));
    commandBuffer.setScissor(0, 1, &scissor);

    mat4 p = cam->getProjection();
    mat4 v = cam->getView();
    mat4 m;

    commandBuffer.pushConstants<mat4>(pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, { p,v,m });

    drawNode(root, mat4());

    commandBuffer.writeTimestamp(vk::PipelineStageFlagBits::eAllCommands, queryPool, 1);
    commandBuffer.endRenderPass();
    commandBuffer.end();

  }

  virtual void init() {

    //root = NeiVu::Loader::scene(RESOURCE_DIR"Models/Challenge/cross.fbx");
    root = NeiVu::Loader::scene(RESOURCE_DIR"Models/Sponza/sponza.fbx");
    //root = NeiVu::Loader::scene(RESOURCE_DIR"Models/texCube/texCube.obj");

    vu->flushCommandBuffer();

    vk::DescriptorSetLayoutBinding dslb;
    dslb.binding = 1;
    dslb.descriptorCount = 1;
    dslb.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    dslb.stageFlags = vk::ShaderStageFlagBits::eFragment;

    vk::DescriptorSetLayoutCreateInfo dslci;
    dslci.bindingCount = 1;
    dslci.pBindings = &dslb;
    descriptorSetLayout = vu->device.createDescriptorSetLayout(dslci);

    vk::PushConstantRange pc;
    pc.offset = 0;
    pc.size = sizeof(mat4) * 3;
    pc.stageFlags = vk::ShaderStageFlagBits::eVertex;

    vk::PipelineLayoutCreateInfo plci;
    plci.setLayoutCount = 1;
    plci.pSetLayouts = &descriptorSetLayout;
    plci.pushConstantRangeCount = 1;
    plci.pPushConstantRanges = &pc;
    pipelineLayout = vu->device.createPipelineLayout(plci);

    createPipeline();

    createDescriptors(root);
    
    vk::QueryPoolCreateInfo cpci;
    cpci.queryType = vk::QueryType::eTimestamp;
    cpci.queryCount = 2;
    queryPool = vu->device.createQueryPool(cpci);

    vk::CommandBufferAllocateInfo cbai;
    cbai.commandPool = vu->commandPool;
    cbai.commandBufferCount = swapchain->swapchainImages.size();
    commandBuffer = vu->device.allocateCommandBuffers(cbai)[0];
  }

  virtual void update(float dt) {
    //mainWindow->setTitle(std::to_string(1.0f / dt) + "");
    manipulator->update(dt);
    if (frame % 10 == 0) {
      mainWindow->setTitle(std::to_string(fps) + "("+std::to_string(1.0f / dt)+")");
    }
  }

  virtual void draw() {


    swapchain->next();

    createDrawCommand();

    vk::PipelineStageFlags flags = vk::PipelineStageFlagBits::eBottomOfPipe;
    vk::SubmitInfo si;
    si.waitSemaphoreCount = (1);
    si.pWaitSemaphores = (&swapchain->semaphore);
    si.pWaitDstStageMask = (&flags);
    si.commandBufferCount = (1);
    si.pCommandBuffers = (&commandBuffer);

    vu->queue.submit(si, 0);
    swapchain->swap();
    uint64 data[2];
    vu->device.getQueryPoolResults(queryPool, 0, 2, sizeof(uint64) * 2, data, sizeof(uint64), vk::QueryResultFlagBits::e64 | vk::QueryResultFlagBits::eWait);
    fps = 1e9 / (data[1] - data[0]);  
    frame++;
  }


};

int main(int /*argc*/, char ** /*argv*/) {
  fgl::BaseApp::options.minimumVersion = 0;
  VuTest app;

  return app.run();
}