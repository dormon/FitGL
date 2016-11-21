#include <BaseApp.h>

#define BALL_RADIUS 1.0f
#define BALL_TESS_X 20
#define BALL_TESS_Y 20
#define BALL_COUNT 16

#define TABLE_WIDTH 50.0f
#define TABLE_DEPTH 30.0f
#define TABLE_HEIGHT 5.0f
#define TABLE_GREEN_HEIGHT BALL_RADIUS*1.5f
#define TABLE_GREEN_BORDER 2.0f
#define TABLE_GREEN_SHIFT 0.3f
#define TABLE_WOOD_BORDER 3.0f
#define TABLE_BOTTOM (-BALL_RADIUS+TABLE_GREEN_HEIGHT-TABLE_HEIGHT)

#define LEG_HEIGHT 10.0f
#define LEG_DISTANCE 0.45f

#define GROUND_SIZE 150.0f

struct Ball {
	glm::vec2 pos;
	glm::vec2 speed;
	glm::mat4 m;
};

class PGP2016 :public BaseApp {
public:
	PGP2016() {};
	virtual ~PGP2016() {};

protected:

	virtual void init();
	virtual void draw();

protected:
	void initPrograms();
	void initGeometry();

	void du01_initIndirectBuffer();
	void du01_multidraw();
	void du02_draw();
	void du03_initFramebuffer();
	void du03_resizeFramebuffer(int w, int h);
	int du03_pick();
	void du03_onClick();
	void du03_update(float dt);

	void du05_init();
	void du05_update();


	virtual void update(float dt);

	virtual void onResize(int w, int h);
	virtual void onMouseMove(int dx, int dy, int x, int y);
	virtual void onMousePress(Uint8 button, int x, int y);
	virtual void onMouseRelease(Uint8 button, int x, int y);
	virtual void onKeyPress(SDL_Keycode key, Uint16 mod);

	void createSphereGeometry(std::vector<Vertex> &vertexData);
	void createBallData();
	void createTableGeometry(std::vector<Vertex> &vertexData);
	void createGroundGeometry(std::vector<Vertex> &vertexData);
	void quad(std::vector<Vertex> &vertexData, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
		glm::vec2 tc0 = glm::vec2(0, 0), glm::vec2 tc1 = glm::vec2(0, 0),
		glm::vec2 tc2 = glm::vec2(0, 0), glm::vec2 tc3 = glm::vec2(0, 0));

	PerspectiveCamera camera;
	OrbitManipulator manipulator;
	glm::vec3 lightPosition = glm::vec3(100, 50, 100);
	bool useComputeShader = false;
	bool usePicking = false;

	bool wireframe = false;
	bool showVolumes = false;

	int mouseX = 0;
	int mouseY = 0;

	int width = 0;
	int height = 0;
	int pick = 0;

	int sphereVertices = 0;
	int sphereFirstVertex = 0;
	int tableGreenVertices = 0;
	int tableGreenFirstVertex = 0;
	int tableWoodVertices = 0;
	int tableWoodFirstVertex = 0;
	int groundVertices = 0;
	int groundFirstVertex = 0;
	
  std::shared_ptr<ge::gl::Program> programID;
  std::shared_ptr<ge::gl::Program> program;
  std::shared_ptr<ge::gl::Program> programLegs;
  std::shared_ptr<ge::gl::Program> programCompute;
  std::shared_ptr<ge::gl::Program> programShadowVolume;
  std::shared_ptr<ge::gl::Program> programShadowVolumeLegs;

	GLuint idBuffer;
	GLuint indirectBuffer;
	GLuint vertexBuffer;
	GLuint vertexArray;

	GLuint vertexArrayLegs;
	GLuint vertexBufferLegs;

	GLuint framebuffer;
	GLuint textureID;
	GLuint renderbuffer;

	GLuint ballBuffer[2];
	GLuint activeBuffer = 0;
	Ball* ballData;
};