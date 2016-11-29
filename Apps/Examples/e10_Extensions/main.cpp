#include <BaseApp.h>
#include <string>
using namespace fgl;

int main(int /*argc*/, char ** /*argv*/) {
  BaseApp app;

  app.addInitCallback([&] {
    int count;
    app.glGetIntegerv(GL_NUM_EXTENSIONS, &count);
    for (int i = 0; i < count; i++) {
      auto a = app.glGetStringi(GL_EXTENSIONS, i);
      std::cout << a << "\n";
    }
  });

  return app.run();
}
