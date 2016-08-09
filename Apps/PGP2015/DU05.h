#pragma once

#include "SimpleApp.h"
#include "glm/glm.hpp"

// Makro pro zarovnani pameti
#if __cplusplus>=201103l
#define GPUALIGN(X) alignas(16) X
#else
#ifdef _MSC_VER
#define GPUALIGN(X) __declspec(align(16)) X
#else
#define GPUALIGN(X) X __attribute__((aligned(16)))
#endif
#endif

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tc;
};

struct SimVertex {
	GPUALIGN(glm::vec3 position);
	GPUALIGN(glm::vec3 normal);
	GPUALIGN(glm::vec2 tc);
	GPUALIGN(glm::vec3 velocity);
	GPUALIGN(Uint32 connections[16]);
	GPUALIGN(float lengths[16]);
};

/* Pokud to prekladac nezna, potreba naskladat vycpavky
struct SimVertex {
	glm::vec3 position;
	float pad1;
	glm::vec3 normal;
	float pad2;
	glm::vec2 tc;
	float pad3[2];
	glm::vec3 velocity;
	float pad4;
	int connections[16];
	float lengths[16];
};*/

class DU05 : public SimpleApp {
public:
	DU05();
protected:
	/**
	* Init funkce, vola se jednou pri startu aplikace
	*/
	virtual void init();

	/**
	* Draw funkc, vola se v kazdem snimku
	*/
	virtual void draw();

	/* Input Eventy */
	virtual void onKeyPress(SDL_Keycode key, Uint16 mod);

	/* Tvorba sceny */
	void createGround();
	void createPole();
	void createFlag();
	void createSM();

	/* Simulace latky */
	void updateScene(int dt);

	/* Vykresleni sceny */
	void drawScene(GLuint program);

	/* Svetlo */
	glm::vec3 lightPosition;

	/* Animace */
	bool animationEnabled = true;
	Uint32 time =0;
	Uint32 lastFrameTics = 0;;

	bool gravityEnabled = true;
	bool windEnabled = true;

	/* Debug */
	bool debug=false;
	bool wireframe=false;

	/* Zem */
	GLuint texGround;
	GLuint vaoGround;
	GLuint vboGround;
	
	/* Stozar */
	int countPole;
	GLuint vaoPole;
	GLuint vboPole;
	GLuint eboPole;

	/* Vlajka */
	int activeBuffer =0;	
	int countFlag;
	int flagVertexCount;
	GLuint texFlag;
	GLuint vaoFlag[2];
	GLuint vboFlag[2];
	GLuint eboFlag;

	/* Shadow mapa */
	int smSize;
	GLuint texSM;
	GLuint fbo;
	glm::mat4 shadowMat;
	glm::mat4 shadowMatBias;

	/* Shadery */
	GLuint drawProgram;
	GLuint computeProgram;
	GLuint smProgram;

	std::string texturePrefix;
};