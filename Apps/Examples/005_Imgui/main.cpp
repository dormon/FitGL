#include <BaseApp.h>

#include <Loader.h>
#include <bunny.h>

#include <Gui.h>

using namespace glm;

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
	GLuint vao,vbo,ebo;
	ProgramObject program;
	
	auto mainWindow = app.getMainWindow();
	auto mainWindowHandle = mainWindow->getWindowHandle();

	float lightDir = 0;
	float zoom = 4;
	float bunnyColor[3] = { 1.0,0.5,0.25};
	bool showDemo = false;

	std::string prefix = "../../Resources/Shaders/Examples/005_Imgui/";
	app.addInitCallback([&]() {
		auto vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "lambert.vert"));
		auto fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "lambert.frag"));
		program = createProgram(vs, fs);

		bunnyInit(vao, vbo, ebo);
		ImGui_ImplSdlGL3_Init(mainWindowHandle);
	});

	app.addEventCallback([] (SDL_Event &e){
		ImGui_ImplSdlGL3_ProcessEvent(&e);
	});

	app.addDrawCallback([&]() {
		int w = mainWindow->getWidth();
		int h = mainWindow->getHeight();
		glViewport(0, 0, w, h); 
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//bunny
		mat4 p = perspective(radians(45.f), float(w) / float(h), 0.1f, 1000.f);
		mat4 v = lookAt(zoom*vec3(-1,1,1), vec3(0, 0, 0), vec3(0, 1, 0));
		float lightDist = 50;
		vec3 lightPos(cos(lightDir), 1, sin(lightDir));
		lightPos = lightPos*lightDist;
		program.use();
		program.set3fv("color", bunnyColor);
		program.setMatrix4fv("p", value_ptr(p));
		program.setMatrix4fv("v", value_ptr(v));
		program.set3fv("lightPosition", value_ptr(lightPos));

		bunnyDraw();

		// GUI
		ImGui_ImplSdlGL3_NewFrame(mainWindowHandle);
		using namespace ImGui;
		label("FPS: " + std::to_string(GetIO().Framerate));		

		Begin("Options",0,ImVec2(200,200));
		SetWindowPos(ImVec2(20, 40));

		ImGui::SliderFloat("Light dir", &lightDir, 0.0f, 2*pi<float>());
		ImGui::ColorEdit3("Bunny color", bunnyColor);

		if (Button("show demo menu")) showDemo ^= 1;
		End();

		if (showDemo) {
			ImGui::ShowTestWindow(&showDemo);
		}
		
		ImGui::Render();

	});
	return app.run();
}
