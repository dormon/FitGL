#include <BaseApp.h>

#include <geGL/StaticCalls.h>
using namespace ge::gl;
using namespace fgl;
using namespace glm;

int main(int /*argc*/, char ** /*argv*/) {
  BaseApp app;
  ProgramS program;

  auto mainWindow = app.getMainWindow();

  std::string prefix = app.getResourceDir() + "Shaders/Examples/e06_ModelLoader/";

  PerspectiveCameraS cam = newPerspectiveCamera();
  OrbitManipulator manipulator(cam);
  manipulator.setupCallbacks(app);
  
  NodeS root, sphere, sphere2;
  float rot = 0;
  app.addInitCallback([&]() {
    auto vs = newShader(GL_VERTEX_SHADER, Loader::text(prefix + "phong.vert"));
    auto fs = newShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "phong.frag"));
    program = newProgram(vs, fs);

    root = newNode();
    
    auto mesh = newMesh();
    auto mat = newPhongMaterial();
    mat->diffuse = vec4(1, 1, 1, 1);
    mesh->addDrawable(Quad(vec2(10,10)).toGeometry(), mat);
    root->addMesh(mesh);

    sphere = newNode();
    sphere->setMatrix(translate(vec3(0, 3, 0)));
    auto mesh2 = newMesh();
    mesh2->addDrawable(Sphere().toGeometry(), mat);
    sphere->addMesh(mesh2);
    root->addNode(sphere);

    sphere2 = newNode();
    sphere2->setMatrix(translate(vec3(3, 3, 0)));
    auto mesh3 = newMesh();
    mesh3->addDrawable(Sphere(0.2).toGeometry(), mat);
    sphere2->addMesh(mesh3);
    root->addNode(sphere2);
  });

  app.addUpdateCallback([&](float dt) {
    rot += dt;
    mat4 m = rotate(rot,vec3(0,1,0));
    m = translate(m, vec3(3, 3, 0));
    sphere2->setMatrix(m);
  });

  app.addResizeCallback([&](int w, int h) {
    glViewport(0, 0, w, h);
    cam->setAspect(float(w) / float(h));
  });

  app.addDrawCallback([&]() {
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //bunny

    glEnable(GL_CULL_FACE);

    program->use();
    program->setMatrix4fv("p", value_ptr(cam->getProjection()));
    program->setMatrix4fv("v", value_ptr(cam->getView()));

    drawNode(program, root);

    fpsLabel();
    label("Orbit manipulator:\nWSAD - Move center\nEQ - Up/Down\nRMB/LMB drag - rotate\nMMB drag - move center\nWheel - zoom", 0, 20, 200, 200);
  });
  return app.run();
}
