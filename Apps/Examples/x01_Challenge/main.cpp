#include <BaseApp.h>

#include <bunny.h>


using namespace glm;

void resizeID(GLuint fbo, GLuint tex,GLuint renderBuffer, int width, int height) {
	glTextureImage2DEXT(tex, GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
	glNamedRenderbufferStorage(renderBuffer,GL_DEPTH_COMPONENT,width,height);
}

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
	ProgramObject programID; 
	ProgramObject program;
	

	auto mainWindow = app.getMainWindow();
	mainWindow->setSize(1280, 720);
	
	std::string prefix = "../../Resources/Shaders/Examples/x01_Challenge/";
	std::string modelPrefix = "../../Resources/Models/Challenge/";

	PerspectiveCamera cam;
	OrbitManipulator manipulator(&cam);
	manipulator.setupCallbacks(app);

	NodeShared cross, sphere, ring;
	NodeShared root = std::make_shared<Node>();

	std::vector<NodeShared> clickPoints;
	int clickPointsType[5 * 5 * 5] = {};
	GLuint texid,fbo,renderBuffer;
	int lastX = 0, lastY = 0;
	int pick;
	bool turn = 0;


	app.addInitCallback([&]() {
		std::cout << "comp1\n";
		auto vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "phong.vert"));
		auto fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "phong.frag"));
		
		program = createProgram(vs, fs);
		
		program.use();
		
		
		std::cout << "prog1 "<<program.getId()<<"\n";
		
		auto vs1 = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "id.vert"));
		auto fs1 = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "id.frag"));
		
		programID = createProgram(vs1, fs1);
		
		programID.use();
		
		std::cout << "prog2 " << programID.getId() << "\n";
		std::cout << "quit\n";
		
		
		cross = Loader::scene(modelPrefix + "cross.fbx");
		sphere = Loader::scene(modelPrefix + "sphere.fbx");
		((PhongMaterial*)(sphere->children[0]->meshes[0]->mat.get()))->diffuse.w = 0.1;
		ring = Loader::scene(modelPrefix + "ring.fbx");


		for (int i = 0; i < 5*5*5; i++){
			auto n = std::make_shared<Node>();
			n->addNode(sphere);
			int x = i % 5;
			int y = (i / 5) % 5;
			int z = i / 25;
			n->setMatrix(translate(mat4(), 6.0f*vec3(x,y,z)-12.0f));
			root->addNode(n);
			clickPoints.push_back(n);
		}
		

		glCreateTextures(GL_TEXTURE_2D,1, &texid);
		glTextureParameteri(texid, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(texid, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glCreateRenderbuffers(1, &renderBuffer);
		resizeID(fbo, texid, renderBuffer, 1280, 720);

		glCreateFramebuffers(1, &fbo);
		glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, texid, 0);
		glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
		GLenum buffers[] = {
			GL_COLOR_ATTACHMENT0
		};
		glNamedFramebufferDrawBuffers(fbo, 1, buffers);
		GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		std::cout << "Framebuffer status " << (status == GL_FRAMEBUFFER_COMPLETE ? "OK" : "error") << std::endl;

	});

	app.addResizeCallback([&](int w, int h) {
		glViewport(0, 0, w, h);
		cam.setAspect(float(w) / float(h));
		resizeID(fbo, texid, renderBuffer, w, h);
	});
	
	app.addMouseMoveCallback([&](int dx,int dy,int x,int y) {
		lastX = x;
		lastY = y;
	});

	app.addMousePressCallback([&] (Uint8 b,int,int){
		if (b == 1 && pick>=0 && clickPointsType[pick]==0) {
			auto n = clickPoints[pick];
			n->removeNode(sphere);
			n->addNode(turn ? cross : ring);
			turn = !turn;
			clickPointsType[pick] = 1;
		}
	});

	app.addDrawCallback([&]() {
		
		glClearColor(0.59, 0.84, 0.91, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		programID.use();
		
		programID.setMatrix4fv("p", value_ptr(cam.getProjection()));
		
		programID.setMatrix4fv("v", value_ptr(cam.getView()));
		
		unsigned int i = 0;
		for (auto &n : clickPoints) {
			programID.set1ui("id", i++);
			drawNode(programID, n);
		}

		unsigned int h = mainWindow->getHeight();
		unsigned int w = mainWindow->getWidth();
		if(lastX>0 && lastY>0 && lastX<w && lastY<h)
		glGetTextureSubImage(texid, 0, lastX, h-lastY, 0, 1, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, 4, &pick);
		if (pick > 5 * 5 * 5)pick = -1;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		program.use();
		program.setMatrix4fv("p", value_ptr(cam.getProjection()));
		program.setMatrix4fv("v", value_ptr(cam.getView()));
		program.set3f("Ld", 1.2, 1.2, 1.2);
		
		program.set1ui("pick",pick);

		for (int i = 0; i < 5 * 5 * 5; i++) {
			auto n = clickPoints.at(i);
			if (clickPointsType[i] == 0) continue;
			program.set1ui("id", i);
			drawNode(program, n);
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		for (int i = 0; i < 5 * 5 * 5; i++) {
			auto n = clickPoints.at(i);
			if (clickPointsType[i] != 0) continue;
			program.set1ui("id", i);
			drawNode(program, n);
		}
		glDepthMask(GL_TRUE);

		glDisable(GL_BLEND);
		
	});
	return app.run();
}
