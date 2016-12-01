#include <BaseApp.h>

#include <geGL/StaticCalls.h>
using namespace fgl;
using namespace ge::gl;

int main(int /*argc*/, char ** /*argv*/) {
  BaseApp app;
  ProgramS program;

  auto mainWindow = app.getMainWindow();

  std::string prefix = app.getResourceDir() + "Shaders/Examples/e06_ModelLoader/";

  PerspectiveCameraS cam = newPerspectiveCamera();
  OrbitManipulatorS manipulator= newOrbitManipulator(cam);
  manipulator->setupCallbacks(app);
  NodeS root;

  GLuint query[2];

  
  app.addInitCallback([&]() {
    auto vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "phong.vert"));
    auto fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "phong.frag"));
    program = createProgram(vs, fs);

    root = Loader::scene(app.getResourceDir() + "Models/sponza/sponza.fbx");
    glCreateQueries(GL_TIMESTAMP, 2, query);
    SDL_GL_SetSwapInterval(0);
  });

  app.addResizeCallback([&](int w, int h) {
    glViewport(0, 0, w, h);
    cam->setAspect(float(w) / float(h));
  });

  app.addDrawCallback([&]() {


    glQueryCounter(query[0], GL_TIMESTAMP);
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //bunny

    program->use();
    program->setMatrix4fv("p", value_ptr(cam->getProjection()));
    program->setMatrix4fv("v", value_ptr(cam->getView()));

    drawNode(program, root);
    glQueryCounter(query[1], GL_TIMESTAMP);

    GLuint64 time1, time2;
    glGetQueryObjectui64v(query[0], GL_QUERY_RESULT, &time1);
    glGetQueryObjectui64v(query[1], GL_QUERY_RESULT, &time2);

    std::string s = "fps: " + std::to_string(1e9 / (time2 - time1)) + " (" + std::to_string(ImGui::GetIO().Framerate) + ")";
    label(s, 0, 0, 300, 100);
  });
  return app.run();
}
