#include <VuApp.h>

using namespace NeiVu;

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp::options.minimumVersion = 0;
	VuApp app;

	return app.run();
}