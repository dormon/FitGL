#include <BaseApp.h>
#include <string>

int main(int /*argc*/, char ** /*argv*/) {
  BaseApp app;

  app.addInitCallback([&] {
    int count;
    glGetIntegerv(GL_NUM_EXTENSIONS, &count);
    for (int i = 0; i < count; i++) {
      auto a = glGetStringi(GL_EXTENSIONS, i);
      std::cout << a << "\n";
    }
  });

  return app.run();
}
