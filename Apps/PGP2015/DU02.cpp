#include "DU02.h"

#include <algorithm>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

glm::vec3 directions[6] = {
	glm::vec3(0,0,-1),
	glm::vec3(0,0,1),
	glm::vec3(1,0,0),
	glm::vec3(-1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,-1,0),
};

DU02::DU02(){
	resizable = false;
	camera.rotx = 20;
	camera.roty = 45;
	camera.button = SDL_BUTTON_RIGHT;
}

void DU02::init() {
	/*
	* Vytvoreni shader programu
	*/

	string shaderPrefix = RESOURCE_DIR"Shaders/PGP2015/";
	string texturePrefix = RESOURCE_DIR"Textures/PGP2015/";

	/*
	* Kompilace shaderu
	*/
	vs = compileShader(GL_VERTEX_SHADER, loadFile(shaderPrefix + "du02_cube.vert"));
	gs = compileShader(GL_GEOMETRY_SHADER, loadFile(shaderPrefix + "du02_cube.geo"));
	fs = compileShader(GL_FRAGMENT_SHADER, loadFile(shaderPrefix + "du02_cube.frag"));
	programCubes = linkShader(3, vs, gs, fs);

	vs1 = compileShader(GL_VERTEX_SHADER, loadFile(shaderPrefix + "du02_quad.vert"));
	gs1 = compileShader(GL_GEOMETRY_SHADER, loadFile(shaderPrefix + "du02_quad.geo"));
	fs1 = compileShader(GL_FRAGMENT_SHADER, loadFile(shaderPrefix + "du02_quad.frag"));
	programView = linkShader(3, vs1, gs1, fs1);

	// zisakni atributu
	positionAttrib = glGetAttribLocation(programCubes, "position");
	// ziskani uniform
	mUniform = glGetUniformLocation(programCubes, "m");
	vUniform = glGetUniformLocation(programCubes, "v");
	pUniform = glGetUniformLocation(programCubes, "p");
	selectedFaceUniform = glGetUniformLocation(programView, "selectedFace");
	selectedCubeUniform = glGetUniformLocation(programView, "selectedCube");
	

	/*
	*	Kopirovani dat na GPU
	*/

	// Nacteni textury s travou
	textureDirt = textureFromBMP(texturePrefix+"du2.bmp");
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	/* vygenerovani jmena pro VAO a jeho aktivace */
	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao);

	/*
	* Vytvoreni bufferu pro pozice kostek
	*/
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*MAX_CUBES, NULL, GL_STATIC_DRAW);
	// vlozeni nekolika kostek
	for (int i = 0; i < 9; i++) {
		insertCube(glm::vec3(i/3-1, 0, i%3-1));
	}
	/*
	* Aktivace atributu
	*/
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);

	/* deaktivovani VAO */
	glBindVertexArray(0);



	
	/* 
	* Vytvoreni G-Bufferu
	*/

	/**** STUDENT DOPLNI ****/
	/*
	* Potreba vytvorit 4 textury velikosti obrazvoky (width,height)
	* texturePosition - float, 3 kanály
	* textureNormal - float, 3 kanály
	* textureColor - byte, 3 kanály
	* textureId - int, 1 kanál
	* vsem texturam je take nutne nastavit parametr filtrovani
	*
	* napoveda: glGenTextures,glBindTexture,glTexImage2D,glTexParameteri
	*/


	glGenTextures(1, &texturePosition);
	glBindTexture(GL_TEXTURE_2D, texturePosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &textureNormal);
	glBindTexture(GL_TEXTURE_2D, textureNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &textureColor);
	glBindTexture(GL_TEXTURE_2D, textureColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	/**** STUDENT DOPLNI ****/
	/* Vytvoreni renderbufferu pro hloubku
	* napoveda: glGenRenderbuffers, glBindRenderbuffer, glRenderbufferStorage
	*/
	glGenRenderbuffers(1, &renderBufferDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);


	/**** STUDENT DOPLNI ****/
	/* Vytvoreni framebufferu
	* pripojeni textur a render bufferu k framebufferu
	* specifikace color bufferu
	* kontrola framebufferu
	*
	* napoveda: glGenFramebuffers,glBindFramebuffer,glFramebufferTexture,glDrawBuffers,glFramebufferRenderbuffer
	*/
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texturePosition, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, textureNormal, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, textureColor, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, textureId, 0);
	GLenum buffers[] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3
	};
	glDrawBuffers(4, buffers);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferDepth);

	GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	cout << "Framebuffer status " << (status == GL_FRAMEBUFFER_COMPLETE ? "OK": "error") << endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void DU02::draw() {
	glViewport(0, 0,width, height);

	/* aktualizace matic */
	camera.update();

	
	/*
	* Cast prvni - vytvoreni sceny do Gbufferu
	*/


	/* Aktivace framebufferu */
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	/* Vycisteni */
	glClearColor(0, 0, 0, -1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Aktivace depth testu*/
	glEnable(GL_DEPTH_TEST);

	/* Aktivace vertex a array*/
	glBindVertexArray(vao);
	/* Aktivace programu */
	glUseProgram(programCubes);

	/* Nastaveni uniforem do programu*/
	glUniformMatrix4fv(mUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	glUniformMatrix4fv(vUniform, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, glm::value_ptr(camera.projection));

	/* Pripojeni textury */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDirt);

	/* Vykresleni vsech kostek jako body,
	*  Geometry shader se postara o vytvoreni kostek
	*/
	glDrawArrays(GL_POINTS, 0, cubeCount);



	Uint32 id=0;
	int x = camera.posx;
	int y = height - camera.posy;
	/**** STUDENT DOPLNI ****/
	/* Vycteni jednoho pixelu z framebuffer z pozice pod ukazatelem mysi (promenna x,y)
	* Cteme pouze z color bufferu, ktery obsahuje ID
	* hodnotu pixelu nactete do promenne id
	* 
	* napoveda: glReadBuffer,glReadPixels
	*/
	glReadBuffer(GL_COLOR_ATTACHMENT3);
	glReadPixels(x,y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id);


	/* Kontrola, ze mys je v okne a nejaky objekt je pod mysi */
	if (x < 0 || x >= width || y<0 || y>height || !(id >> 3)) {
		selected = false;
		cubeId = -1;
		faceId = -1;
		//cout << "Selection: nothing"<<endl;
	}	else {
		selected = true;
		cubeId = (id >> 3) - 1;
		faceId = id & 7;
		//cout << "Selection: "<< cubeId <<"-"<<faceId << endl;
	}

	
	/*
	* Cast druha - vykresleni obdelniku pres celou obrazovku a doplneni osvteleni
	*/

	/* aktivace defaultniho framebufferu */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/* Aktivace prazdneho vertex array */
	glBindVertexArray(vao1);
	/* vypnuti depth testu*/
	glDisable(GL_DEPTH_TEST);
	/* aktivace programu*/
	glUseProgram(programView);

	/* Pripojeni textur */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureColor);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureId);

	/* Nastaveni uniforem*/
	glUniform1ui(selectedFaceUniform, faceId);
	glUniform1ui(selectedCubeUniform, cubeId);

	/* Vykresleni obdelniku (geometry shader) */
	glDrawArrays(GL_POINTS, 0, 1);
}

void DU02::onMousePress(Uint8 button, int x, int y){
	switch (button) {
	case SDL_BUTTON_LEFT:
		if (selected) {
			insertCube(cubes[cubeId] + directions[faceId]);
		}
		break;
	}
}

void DU02::onKeyPress(SDL_Keycode key, Uint16 mod){
	switch (key) {
	case SDLK_ESCAPE:
		quit();
	}
}

void DU02::insertCube(glm::vec3 pos){
	if (cubeCount >= MAX_CUBES) return;
	glBufferSubData(GL_ARRAY_BUFFER, cubeCount*cubeSize, cubeSize, glm::value_ptr(pos));
	cubes.push_back(pos);
	cubeCount++;
}

int main(int /*argc*/, char ** /*argv*/){
	DU02 app;
	return app.run();
}