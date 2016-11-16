#include <BaseApp.h>

#include <Loader.h>
#include <bunny.h>

#include <Gui.h>

using namespace glm;
using namespace std;

int main(int /*argc*/, char ** /*argv*/) {
  BaseApp app;
  ProgramObject programPlanet;
  ProgramObject programAtmosphere;
  ProgramObject programStars;


  auto mainWindow = app.getMainWindow();

  PerspectiveCamera cam;
  OrbitManipulator manipulator(&cam);
  manipulator.setupCallbacks(app);

  GLuint vao;
  GLuint vbo;

  bool wireframe = false;
  int starCount = 20000;
  int seed = 0;

  app.addInitCallback([&]() {
    string prefix = app.getResourceDir() + "shaders/Nei/n01_Planet/";
    auto vert = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "empty.vert"));
    auto tesc = compileShader(GL_TESS_CONTROL_SHADER, Loader::text(prefix + "planet.tesc"));
    auto tese = compileShader(GL_TESS_EVALUATION_SHADER, Loader::text(prefix + "planet.tese"));
    auto frag = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "planet.frag"));
    programPlanet = createProgram(vert, tesc, tese, frag);
    
    vert = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "empty.vert"));
    tesc = compileShader(GL_TESS_CONTROL_SHADER, Loader::text(prefix + "atmosphere.tesc"));
    tese = compileShader(GL_TESS_EVALUATION_SHADER, Loader::text(prefix + "atmosphere.tese"));
    frag = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "atmosphere.frag"));
    programAtmosphere = createProgram(vert, tesc, tese, frag);
    
    vert = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "stars.vert"));
    frag = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "stars.frag"));
    programStars = createProgram(vert, frag);

    glCreateVertexArrays(1, &vao);
    float distance = 400;

    vector<vec4> vec;
    vec.reserve(starCount);
    for (int i = 0; i<starCount; i++) {
      float a = ((float)rand() / RAND_MAX - 0.5);
      float b = ((float)rand() / RAND_MAX - 0.5);
      float c = ((float)rand() / RAND_MAX - 0.5);
      float d = (float)rand() / RAND_MAX * 5 + 1;

      vec4 v(a, b, c, 0);
      v = normalize(v);
      v *= distance;
      v.w = d;

      vec.push_back(v);
    }

    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(vec4)*vec.size(), vec.data(), GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(vec4));
    glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, 0, 0);
    glEnableVertexArrayAttrib(vao, 0);
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

    programStars.use();
    programStars.setMatrix4fv("v", value_ptr(cam.getView()));
    programStars.setMatrix4fv("p", value_ptr(cam.getProjection()));

    glEnable(GL_PROGRAM_POINT_SIZE);    
    glDrawArrays(GL_POINTS, 0, starCount);

    
    programPlanet.use();
    programPlanet.setMatrix4fv("v", value_ptr(cam.getView()));
    programPlanet.setMatrix4fv("p", value_ptr(cam.getProjection()));
    programPlanet.set3fv("camPos", value_ptr(cam.getEye()));
    programPlanet.set1i("seed", seed);
    programPlanet.set1f("time", app.getTimeFromStart());

    glPatchParameteri(GL_PATCH_VERTICES, 1);
    glDrawArraysInstanced(GL_PATCHES, 0, 1, 16);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    programAtmosphere.use();
    programAtmosphere.setMatrix4fv("v", value_ptr(cam.getView()));
    programAtmosphere.setMatrix4fv("p", value_ptr(cam.getProjection()));
    programAtmosphere.set3fv("camPos", value_ptr(cam.getEye()));
    programAtmosphere.set1ui("seed", seed);
    programAtmosphere.set1f("time", app.getTimeFromStart());

    glPatchParameteri(GL_PATCH_VERTICES, 1);
    glDrawArrays(GL_PATCHES, 0, 1);
  });
  return app.run();
}
