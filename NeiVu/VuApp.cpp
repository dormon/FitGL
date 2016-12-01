#include "VuApp.h"
#include "VuInfo.h"
#include <vector>
#include <algorithm>

#include <SDL2/SDL_syswm.h>

using namespace NeiVu;
using namespace std;

VuApp::VuApp() {
  width = mainWindow->getWidth();
  height = mainWindow->getHeight();

  try {
    vu = new NeiVu::Context();
    NeiVu::Context::makeCurrent(vu);

    SwapchainOptions so;
    so.vu = vu;
    so.window = mainWindow->getWindowHandle();
    swapchain = new Swapchain(so);

    vu->flushCommandBuffer();
  }
  catch (system_error er) {
    cout << "error " << er.what() << "\n";
  }
}
