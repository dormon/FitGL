#include <BaseApp.h>

#include <geGL/StaticCalls.h>
using namespace ge::gl;
using namespace fgl;

std::string vsrc = R".(
#version 450
uniform mat4 v;
uniform mat4 p;

out vec4 vColor;

void main(){
  const uint mapping[] = {0,1,2,2,1,3};
  uint quadVertexId = mapping[gl_VertexID%6];
  uint faceId = gl_VertexID/6;
  float size = .5;
  vec4 position = vec4(size*(-1+2*vec2(quadVertexId%2,quadVertexId/2)),faceId,1);
  gl_Position = p * (v * position);

  if(faceId == 0)
    vColor = vec4(1,0,0,.5);
  else if(faceId == 1)
    vColor = vec4(0,1,0,.3);
  else
    vColor = vec4(0,0,1,.4);
}

).";

std::string fsrc = R".(
#version 450
layout(location=0)out vec4 fColor;

in vec4 vColor;

void main(){
  fColor = vColor;
}
).";

int main(int /*argc*/, char ** /*argv*/) {
  BaseApp app;
  std::shared_ptr<Program> program;
  GLuint vao;
  GLuint vbo;
  auto mainWindow = app.getMainWindow();

  PerspectiveCameraS cam = newPerspectiveCamera();
  OrbitManipulator manipulator(cam);
  manipulator.setupCallbacks(app);

  app.addInitCallback([&]() {
    auto vs = compileShader(GL_VERTEX_SHADER, vsrc);
    auto fs = compileShader(GL_FRAGMENT_SHADER, fsrc);
    program = createProgram(vs, fs);
    glCreateVertexArrays(1, &vao);


    glClearColor(0.2, 0.2, 0.2, 1);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  });

  app.addResizeCallback([&](int w, int h) {
    glViewport(0, 0, w, h);
    cam->setAspect(float(w) / float(h));
  });

  app.addDrawCallback([&]() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program->use();

    program->setMatrix4fv("p", value_ptr(cam->getProjection()));
    program->setMatrix4fv("v", value_ptr(cam->getView()));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 18);
  });
  return app.run();
}