#include <BaseApp.h>

#include <Loader.h>
#include <bunny.h>

#include <Gui.h>

using namespace glm;
using namespace std;

int main(int /*argc*/, char ** /*argv*/) {
  BaseApp app;
  ProgramObject beamProgram;

  auto mainWindow = app.getMainWindow();

  PerspectiveCamera cam;
  OrbitManipulator manipulator(&cam);
  manipulator.setupCallbacks(app);
  manipulator.setRotationX(180);
  manipulator.setRotationY(90);
  manipulator.setZoom(50);

  GLuint vao;
  GLuint vbo;

  bool wireframe = false;
  int starCount = 20000;
  int seed = 0;
  bool pause = false;

  app.addInitCallback([&]() {
    string prefix = app.getResourceDir() + "shaders/Nei/n02_Beams/";
    auto vert = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "beam.vert"));
    auto geo = compileShader(GL_GEOMETRY_SHADER, Loader::text(prefix + "beam.geo"));
    auto frag = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "beam.frag"));
    beamProgram = createProgram(vert, geo, frag);

    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
  });
  app.addUpdateCallback([&](float dt) {
    manipulator.update(dt);
  });
  app.addDrawCallback([&]() {
    int w = mainWindow->getWidth();
    int h = mainWindow->getHeight();
    glViewport(0, 0, w, h);

    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    beamProgram.use();
    beamProgram.setMatrix4fv("v", value_ptr(cam.getView()));
    beamProgram.setMatrix4fv("p", value_ptr(cam.getProjection()));
    beamProgram.set3fv("camPos", value_ptr(cam.getEye()));
    beamProgram.set1f("time", app.getTimeFromStart());
  
    for (int i = 0; i<10; i++) {

      beamProgram.set3f("beamStart", -10, 0, -20 + i * 5);
      beamProgram.set3f("beamEnd", 10, 0, -20 + i * 5);
      beamProgram.set1f("beamWidth", 1.f);
      beamProgram.set1i("type", i);
      beamProgram.set1i("pause", pause);

      if (i == 3 || i == 7)beamProgram.set1f("beamWidth", 3.f);

      glDrawArrays(GL_POINTS, 0, 1);
    }

    fpsLabel();
  });

  app.addKeyPressCallback([&](SDL_Keycode k,Uint16) {
    if (k == SDLK_SPACE)pause = !pause;
  });

  return app.run();
}
