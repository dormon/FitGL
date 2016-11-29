#include "pgp2016.h"


using namespace glm;
using namespace fgl;

void PGP2016::init() {
  ge::gl::Program::setNonexistingUniformWarning(false);
  camera = newPerspectiveCamera();
  manipulator = newOrbitManipulator(camera);
	manipulator->setupCallbacks(*this);
	manipulator->setRotationY(45);
	manipulator->setZoom(50);

	initPrograms();
	initGeometry();

	du01_initIndirectBuffer();

	du03_initFramebuffer();

	du05_init();

	glClearColor(0.59, 0.84, 0.91, 1);

}

void PGP2016::initPrograms() {
	std::string prefix = getResourceDir() + "Shaders/PGP2016/";

	auto vs = compileShader(GL_VERTEX_SHADER,
		"#version 430\n",
		Loader::text(prefix + "du01.vert"));
	auto fs = compileShader(GL_FRAGMENT_SHADER,
		"#version 430\n",
		Loader::text(prefix + "shading.glsl"),
		Loader::text(prefix + "materials.glsl"),
		Loader::text(prefix + "du01.frag"));
	program = createProgram(vs, fs);

	vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "du02.vert"));
	auto tesc = compileShader(GL_TESS_CONTROL_SHADER, Loader::text(prefix + "du02.tesc"));
	auto tese = compileShader(GL_TESS_EVALUATION_SHADER, Loader::text(prefix + "du02.tese"));
	fs = compileShader(GL_FRAGMENT_SHADER,
		"#version 430\n",
		Loader::text(prefix + "shading.glsl"),
		Loader::text(prefix + "materials.glsl"),
		Loader::text(prefix + "du02.frag"));
	programLegs = createProgram(vs, tesc, tese, fs);


	vs = compileShader(GL_VERTEX_SHADER,
		Loader::text(prefix + "du03.vert"));
	fs = compileShader(GL_FRAGMENT_SHADER,
		Loader::text(prefix + "du03.frag"));
	programID = createProgram(vs, fs);

	vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "du04.vert"));
	auto geo = compileShader(GL_GEOMETRY_SHADER, Loader::text(prefix + "du04.geo"));
	fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "du04.frag"));
	programShadowVolume = createProgram(vs, geo, fs);


	vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "du02.vert"));
	tesc = compileShader(GL_TESS_CONTROL_SHADER, Loader::text(prefix + "du02.tesc"));
	tese = compileShader(GL_TESS_EVALUATION_SHADER, Loader::text(prefix + "du02.tese"));
	geo = compileShader(GL_GEOMETRY_SHADER, Loader::text(prefix + "du04.geo"));
	fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "du04.frag"));
	programShadowVolumeLegs = createProgram(vs, tesc, tese, geo, fs);

	auto cs = compileShader(GL_COMPUTE_SHADER, Loader::text(prefix + "du05.comp"));
	programCompute = createProgram(cs);
}

void PGP2016::initGeometry() {
	std::vector<Vertex> vertexData;
	createSphereGeometry(vertexData);
	createGroundGeometry(vertexData);
	createTableGeometry(vertexData);

	std::vector<int> idData;
	idData.push_back(0);
	idData.push_back(1);
	idData.push_back(2);
	idData.push_back(3);
	idData.push_back(4);
	glGenBuffers(1, &idBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, idBuffer);
	glBufferData(GL_ARRAY_BUFFER, idData.size() * sizeof(int), idData.data(), GL_STATIC_DRAW);
	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);
	//glCreateBuffers(1, &vertexBuffer);
	//glNamedBufferData(vertexBuffer, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(Vertex, norm));
	glVertexAttribPointer(2, 2, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(Vertex, tc));
	glBindBuffer(GL_ARRAY_BUFFER, idBuffer);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 1, GL_INT, sizeof(int), 0);
	glVertexAttribDivisor(3, BALL_COUNT);
	glBindVertexArray(0);

	/*glCreateVertexArrays(1, &vertexArray);
	glVertexArrayVertexBuffer(vertexArray, 0, vertexBuffer, offsetof(Vertex, pos), sizeof(Vertex));
	glVertexArrayVertexBuffer(vertexArray, 1, vertexBuffer, offsetof(Vertex, norm), sizeof(Vertex));
	glVertexArrayVertexBuffer(vertexArray, 2, vertexBuffer, offsetof(Vertex, tc), sizeof(Vertex));
	glVertexArrayAttribFormat(vertexArray, 0, 3, GL_FLOAT, 0, 0);
	glVertexArrayAttribFormat(vertexArray, 1, 3, GL_FLOAT, 0, 0);
	glVertexArrayAttribFormat(vertexArray, 2, 2, GL_FLOAT, 0, 0);
	glEnableVertexArrayAttrib(vertexArray, 0);
	glEnableVertexArrayAttrib(vertexArray, 1);
	glEnableVertexArrayAttrib(vertexArray, 2);*/

	glGenBuffers(2, ballBuffer);
	//glCreateBuffers(2, ballBuffer);
	createBallData();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ballBuffer[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Ball)*BALL_COUNT, ballData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ballBuffer[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Ball)*BALL_COUNT, ballData, GL_DYNAMIC_DRAW);

	//glNamedBufferData(ballBuffer[0], sizeof(Ball)*BALL_COUNT, ballData, GL_DYNAMIC_DRAW);
	//glNamedBufferData(ballBuffer[1], sizeof(Ball)*BALL_COUNT, ballData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ballBuffer[activeBuffer]);


	vec3 legData[] = {
		vec3(TABLE_WIDTH*LEG_DISTANCE,TABLE_BOTTOM,TABLE_DEPTH*LEG_DISTANCE),
		vec3(-TABLE_WIDTH*LEG_DISTANCE,TABLE_BOTTOM,TABLE_DEPTH*LEG_DISTANCE),
		vec3(-TABLE_WIDTH*LEG_DISTANCE,TABLE_BOTTOM,-TABLE_DEPTH*LEG_DISTANCE),
		vec3(TABLE_WIDTH*LEG_DISTANCE,TABLE_BOTTOM,-TABLE_DEPTH*LEG_DISTANCE)
	};
	//glCreateBuffers(1, &vertexBufferLegs);		
	//glNamedBufferData(vertexBufferLegs, sizeof(legData), legData, GL_STATIC_DRAW);
	glGenBuffers(1, &vertexBufferLegs);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferLegs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(legData), legData, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArrayLegs);
	glBindVertexArray(vertexArrayLegs);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(vec3), 0);
	/*glCreateVertexArrays(1, &vertexArrayLegs);
	glVertexArrayVertexBuffer(vertexArrayLegs, 0, vertexBufferLegs, 0, sizeof(vec3));
	glVertexArrayAttribFormat(vertexArrayLegs, 0, 3, GL_FLOAT, 0, 0);
	glEnableVertexArrayAttrib(vertexArrayLegs, 0);*/
}

void PGP2016::draw() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	pick = du03_pick();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	program->use();
	program->setMatrix4fv("v", value_ptr(camera->getView()));
	program->setMatrix4fv("p", value_ptr(camera->getProjection()));
	program->set3fv("cameraPosition", value_ptr(camera->getEye()));
	program->set3fv("lightPosition", value_ptr(lightPosition));
	program->set1i("pick", pick);
	program->set3f("La", 0.4, 0.4, 0.4);
	program->set3f("Ld", 0, 0, 0);

	du01_multidraw();


	programLegs->use();
	programLegs->setMatrix4fv("tesView", value_ptr(camera->getView()));
	programLegs->setMatrix4fv("tesProj", value_ptr(camera->getProjection()));
	programLegs->set3fv("cameraPosition", value_ptr(camera->getEye()));
	programLegs->set3fv("lightPosition", value_ptr(lightPosition));
	programLegs->set1f("legHeight", LEG_HEIGHT);
	programLegs->set3f("La", 0.4, 0.4, 0.4);
	programLegs->set3f("Ld", 0, 0, 0);

	du02_draw();

	glDepthFunc(GL_LESS);
	glDepthMask(GL_FALSE);

	glDisable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0);
	if(!showVolumes)glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);

	programShadowVolume->use();
	programShadowVolume->setMatrix4fv("v", value_ptr(camera->getView()));
	programShadowVolume->setMatrix4fv("p", value_ptr(camera->getProjection()));
	programShadowVolume->set3fv("lightPosition", value_ptr(lightPosition));

	du01_multidraw();


	programShadowVolumeLegs->use();

	programShadowVolumeLegs->setMatrix4fv("tesView", value_ptr(mat4()));
	programShadowVolumeLegs->setMatrix4fv("tesProj", value_ptr(mat4()));
	programShadowVolumeLegs->set3fv("lightPosition", value_ptr(lightPosition));
	programShadowVolumeLegs->setMatrix4fv("v", value_ptr(camera->getView()));
	programShadowVolumeLegs->setMatrix4fv("p", value_ptr(camera->getProjection()));
	programShadowVolumeLegs->set1f("legHeight", LEG_HEIGHT);

	du02_draw();

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glStencilFunc(GL_EQUAL, 0, 0xff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_CULL_FACE);

	program->use();
	program->set1i("pick", -1);
	program->set3f("La", 0, 0, 0);
	program->set3f("Ld", 0.6, 0.6, 0.6);

	du01_multidraw();

	programLegs->use();
	programLegs->set3f("La", 0, 0, 0);
	programLegs->set3f("Ld", 0.6, 0.6, 0.6);

	du02_draw();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);

	textColor(1, 0, 0, 1);
	fpsLabel();
	label("\nX - Wireframe\nC - Show shadow volumes");
}


void PGP2016::update(float dt) {
	dt = 1. / 60.0;
	float w = TABLE_WIDTH / 2 - TABLE_GREEN_SHIFT - BALL_RADIUS;
	float h = TABLE_DEPTH / 2 - TABLE_GREEN_SHIFT - BALL_RADIUS;

	if (useComputeShader) {
		du05_update();
	}
	else {
		du03_update(dt);
	}
}

void PGP2016::onResize(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	camera->setAspect(float(w) / float(h));
	du03_resizeFramebuffer(w, h);
}

void PGP2016::onMouseMove(int dx, int dy, int x, int y) {
	mouseX = x;
	mouseY = y;
}

void PGP2016::onMousePress(Uint8 button, int x, int y) {
	if (button == 1) {
		du03_onClick();
	}
}

void PGP2016::onMouseRelease(Uint8 button, int x, int y) {
}

void PGP2016::onKeyPress(SDL_Keycode key, Uint16 mod){
	switch (key) {
	case SDLK_x:
		wireframe = !wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE: GL_FILL);
		break;
	case SDLK_c:
		showVolumes = !showVolumes;
		break;
	}
}

void PGP2016::createSphereGeometry(std::vector<Vertex>& vertexData) {
	const int vertiesPerFace = 6;
	sphereVertices = BALL_TESS_X*BALL_TESS_Y*vertiesPerFace;
	sphereFirstVertex = vertexData.size();
	float radius = BALL_RADIUS;
	for (int y = 0; y < BALL_TESS_Y; ++y) {
		for (int x = 0; x < BALL_TESS_X; ++x) {
			for (int k = 0; k < vertiesPerFace; ++k) {
				const int xOffset[] = { 0,1,0,0,1,1 };
				const int yOffset[] = { 0,0,1,1,0,1 };
				float u = (float)(x + xOffset[k]) / BALL_TESS_X;
				float v = (float)(y + yOffset[k]) / BALL_TESS_Y;
				float xAngle = -u*two_pi<float>();
				float yAngle = v*pi<float>();
				vec3 normal(cos(xAngle)*sin(yAngle), -cos(yAngle), sin(xAngle)*sin(yAngle));
				vec3 position = normal*radius;
				vec2 texCoord(u, v);
				vertexData.emplace_back(Vertex{ position,normal,texCoord });
			}
		}
	}
}

void PGP2016::createBallData() {
	ballData = new Ball[BALL_COUNT];
	int l = 0;
	int il = 0;
	const float c = 0.5*sqrt(3);

	for (int i = 0; i < BALL_COUNT; i++) {
		ballData[i].pos = vec2(l*c, il - l*0.5)*BALL_RADIUS*2.0f;
		ballData[i].speed = vec2(0);
		ballData[i].m = translate(vec3(ballData[i].pos.x, 0, ballData[i].pos.y));
		ballData[i].m = rotate(ballData[i].m, radians(float(0)), vec3(rand(), rand(), rand()));
		//ballData[i].m = rotate(ballData[i].m, radians(float(rand())), vec3(rand(), rand(), rand()));
		il++;
		if (il == l + 1) {
			l++; il = 0;
		}
	}
	ballData[BALL_COUNT - 1].pos = vec2(-5, 0)*BALL_RADIUS*2.0f;
	ballData[BALL_COUNT - 1].m = translate(vec3(ballData[BALL_COUNT - 1].pos.x, 0, ballData[BALL_COUNT - 1].pos.y));
}

void PGP2016::createTableGeometry(std::vector<Vertex>& vertexData) {
	tableGreenFirstVertex = vertexData.size();
	float x = TABLE_WIDTH*0.5;
	float y = TABLE_DEPTH*0.5;
	float d = -BALL_RADIUS;
	float h = TABLE_GREEN_HEIGHT;
	float s = TABLE_GREEN_SHIFT;
	float x2 = x + TABLE_GREEN_BORDER;
	float y2 = y + TABLE_GREEN_BORDER;
	float x3 = x2 + TABLE_WOOD_BORDER;
	float y3 = y2 + TABLE_WOOD_BORDER;
	float d2 = d + h - TABLE_HEIGHT;

	quad(vertexData, vec3(-x, d, +y), vec3(+x, d, +y), vec3(+x, d, -y), vec3(-x, d, -y));

	quad(vertexData, vec3(+x, d, +y), vec3(-x, d, +y), vec3(-x + s, d + h, +y - s), vec3(+x - s, d + h, +y - s));
	quad(vertexData, vec3(-x, d, -y), vec3(+x, d, -y), vec3(+x - s, d + h, -y + s), vec3(-x + s, d + h, -y + s));
	quad(vertexData, vec3(+x, d, -y), vec3(+x, d, +y), vec3(+x - s, d + h, +y - s), vec3(+x - s, d + h, -y + s));
	quad(vertexData, vec3(-x, d, +y), vec3(-x, d, -y), vec3(-x + s, d + h, -y + s), vec3(-x + s, d + h, +y - s));

	quad(vertexData, vec3(-x2, d + h, +y2), vec3(+x2, d + h, +y2), vec3(+x - s, d + h, +y - s), vec3(-x + s, d + h, +y - s));
	quad(vertexData, vec3(+x2, d + h, -y2), vec3(-x2, d + h, -y2), vec3(-x + s, d + h, -y + s), vec3(+x - s, d + h, -y + s));
	quad(vertexData, vec3(+x2, d + h, +y2), vec3(+x2, d + h, -y2), vec3(+x - s, d + h, -y + s), vec3(+x - s, d + h, +y - s));
	quad(vertexData, vec3(-x2, d + h, -y2), vec3(-x2, d + h, +y2), vec3(-x + s, d + h, +y - s), vec3(-x + s, d + h, -y + s));

	tableGreenVertices = vertexData.size() - tableGreenFirstVertex;
	tableWoodFirstVertex = vertexData.size();
	quad(vertexData, vec3(-x3, d + h, +y3), vec3(+x3, d + h, +y3), vec3(+x2, d + h, +y2), vec3(-x2, d + h, +y2));
	quad(vertexData, vec3(+x3, d + h, -y3), vec3(-x3, d + h, -y3), vec3(-x2, d + h, -y2), vec3(+x2, d + h, -y2));
	quad(vertexData, vec3(+x3, d + h, +y3), vec3(+x3, d + h, -y3), vec3(+x2, d + h, -y2), vec3(+x2, d + h, +y2));
	quad(vertexData, vec3(-x3, d + h, -y3), vec3(-x3, d + h, +y3), vec3(-x2, d + h, +y2), vec3(-x2, d + h, -y2));

	quad(vertexData, vec3(-x3, d + h, +y3), vec3(-x3, d2, +y3), vec3(+x3, d2, +y3), vec3(+x3, d + h, +y3));
	quad(vertexData, vec3(+x3, d + h, -y3), vec3(+x3, d2, -y3), vec3(-x3, d2, -y3), vec3(-x3, d + h, -y3));
	quad(vertexData, vec3(+x3, d + h, +y3), vec3(+x3, d2, +y3), vec3(+x3, d2, -y3), vec3(+x3, d + h, -y3));
	quad(vertexData, vec3(-x3, d + h, -y3), vec3(-x3, d2, -y3), vec3(-x3, d2, +y3), vec3(-x3, d + h, +y3));

	quad(vertexData, vec3(-x3, d2, +y3), vec3(-x3, d2, -y3), vec3(+x3, d2, -y3), vec3(+x3, d2, +y3));

	tableWoodVertices = vertexData.size() - tableWoodFirstVertex;
}

void PGP2016::createGroundGeometry(std::vector<Vertex>& vertexData) {
	groundFirstVertex = vertexData.size();
	groundVertices = 6;
	float s = GROUND_SIZE * 0.5;
	float d = -BALL_RADIUS + TABLE_GREEN_HEIGHT - TABLE_HEIGHT - LEG_HEIGHT;
	quad(vertexData, vec3(-s, d, s), vec3(+s, d, s), vec3(+s, d, -s), vec3(-s, d, -s));
}

void PGP2016::quad(std::vector<Vertex>& vertexData,
	glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
	glm::vec2 tc0, glm::vec2 tc1, glm::vec2 tc2, glm::vec2 tc3) {
	vec3 norm = normalize(cross(p1 - p0, p2 - p0));
	vertexData.emplace_back(Vertex{ p0,norm,tc0 });
	vertexData.emplace_back(Vertex{ p1,norm,tc1 });
	vertexData.emplace_back(Vertex{ p2,norm,tc2 });
	vertexData.emplace_back(Vertex{ p0,norm,tc0 });
	vertexData.emplace_back(Vertex{ p2,norm,tc2 });
	vertexData.emplace_back(Vertex{ p3,norm,tc3 });
}

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp::options.minimumVersion = 430;
	PGP2016 app;
	return app.run();
}