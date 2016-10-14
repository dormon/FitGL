#include "DU01.h"

using namespace std;

/*
* Geometry data
*/

// pozice vrcholu
const float vertexData[] = {
	//0
	-0.4,0.6,
	-0.2,0.6,
	0.,0.6,
	0.2,0.6,
	0.4,0.6,
	//5
	-0.4,0.4,
	-0.2,0.4,
	0.,0.4,
	0.2,0.4,
	0.4,0.4,
	//10
	-0.4,0.2,
	-0.2,0.2,
	0.,0.2,
	0.2,0.2,
	0.4,0.2,
	//15
	-0.4,0.,
	-0.2,0.,
	0.,0.,
	0.2,0.,
	0.4,0.,
	//20
	-0.4,-0.2,
	-0.2,-0.2,
	0.,-0.2,
	0.2,-0.2,
	0.4,-0.2,
	//25
	-0.4,-0.4,
	-0.2,-0.4,
	0.,-0.4,
	0.2,-0.4,
	0.4,-0.4
};

// indexy na vrcholy
const unsigned int indexData[] = {
	// 10 indexu = 8 trojuhelniku
	0,5,1,6,2,7,3,8,4,9,
	// restart index
	666,
	// 10 indexu = 8 trojuhelniku
	10,15,11,16,12,17,13,18,14,19,
	// restart index
	666,
	// 10 indexu = 8 trojuhelniku
	20,25,21,26,22,27,23,28,24,29,
};


void DU01::init() {
	/*
	* Vytvoreni shader programu
	*/

	string prefix = RESOURCE_DIR"Shaders/PGP2015/";

	/*
	* Kompilace shaderu
	*/
	vertexShader = compileShader(GL_VERTEX_SHADER, loadFile(prefix + "du01.vert"));
	fragmentShader = compileShader(GL_FRAGMENT_SHADER, loadFile(prefix + "du01.frag"));
	
	/*
	* Linkovani shaderu, zaroven vypise chyby
	*/
	shaderProgram = linkShader(2, vertexShader, fragmentShader);
	
	// zisakni indexu atributu
	positionAttrib = glGetAttribLocation(shaderProgram, "position");


	/*
	*	Kopirovani dat na GPU
	*/

	/* vygenerovani jmena pro VAO a jeho aktivace */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*
	* Vytvoreni bufferu pro vertex data
	* Alokace bufferu a akopirovani dat
	*/
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	/*
	* Vytvoreni bufferu pro indexy
	* Alokace bufferu a nakopirovani dat
	*/
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	/*
	* Aktivace atributu
	*/
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)(sizeof(float) * 0));
	
	typedef struct {
		Uint32  count;
		Uint32  primCount;
		Uint32  firstIndex;
		Uint32  baseVertex;
		Uint32  baseInstance;
	} DrawElementsIndirectCommand;
	/* Ukol 4  - indirect buffer */
	/**** STUDENT DOPLNI ****/
	/* vytvoreni bufferu pro indirect draw (dibo)
	* Buffer musi mit spravnou velikost
	* data do bufferu nahrajte pomoci mapovani bufferu
	* kreslime tri stripy, kazdy zvlast
	* vykreslime 3 instance
	* napoveda: glGenBuffers,glBindBuffer,glBufferData,glMapBuffer,glUnmapBuffer
	*/
	glGenBuffers(1, &dibo);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, dibo);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand)*3, NULL, GL_STATIC_DRAW);

	DrawElementsIndirectCommand* ptr = (DrawElementsIndirectCommand*)glMapBuffer(GL_DRAW_INDIRECT_BUFFER, GL_READ_WRITE);
	ptr[0].count = 10;
	ptr[0].primCount = 3;
	ptr[0].firstIndex = 0;
	ptr[0].baseVertex = 0;
	ptr[0].baseInstance = 0;
	ptr[1].count = 10;
	ptr[1].primCount = 3;
	ptr[1].firstIndex = 11;
	ptr[1].baseVertex = 0;
	ptr[1].baseInstance = 0;
	ptr[2].count = 10;
	ptr[2].primCount = 3;
	ptr[2].firstIndex = 22;
	ptr[2].baseVertex = 0;
	ptr[2].baseInstance = 0;
	glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);

	/* deaktivovani VAO */
	glBindVertexArray(0);
}

void DU01::draw(){

	/* Vycisteni */
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* Prepnuti na wireframe */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	/* Aktivace programu */
	glUseProgram(shaderProgram);

	/* Aktivace VAO */
	glBindVertexArray(vao);

	/* Ukol 1 - klasicky draw*/
	glViewport(0, height / 2, width / 2, height / 2);

	/**** STUDENT DOPLNI ****/
	/* Vykresleni pomoci glDrawElements
	* Mame 3 nesouvisle stripy, je nutne volat 3 draw prikazy se spravnym poctem a offsetem
	* viz indexData pole nahore
	*/

	glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_INT, (void*)(11 * 4));
	glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_INT, (void*)(22 * 4));


	/* Ukol 2 - primitive restart */
	glViewport(width / 2, height / 2, width / 2, height / 2);

	/**** STUDENT DOPLNI ****/
	/* Vykresleni zase pomoci glDrawElements
	* Vyuzijte primitive restart index k vykresleni 3 stripu jednim draw prikazem
	* viz indexData pole nahore
	*/

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex((unsigned int)666);
	glDrawElements(GL_TRIANGLE_STRIP, 32, GL_UNSIGNED_INT, 0);
	glDisable(GL_PRIMITIVE_RESTART);

	/* Ukol 3 - multidraw */
	glViewport(0, 0, width / 2, height / 2);

	/**** STUDENT DOPLNI ****/
	/* Vykreslete pomoci glMultiDrawElements
	* vychazi z bodu 1, kombinuje 3 draw prikazy do jednoho
	*/
	GLsizei count[3] = { 10,10,10 };
	GLvoid* indices[3] = { (GLvoid*)0, (GLvoid*)44, (GLvoid*)88 };
	glMultiDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, (const GLvoid**)indices, 3);


	/* Ukol 4 - multidraw indirect*/
	glViewport(width / 2, 0, width / 2, height / 2);

	/**** STUDENT DOPLNI ****/
	/* Vykreslete pomoci glMultidrawElementsIndirect
	* stejne jako multidraw, vyuziva ale buffer na gpu
	* nejprve v init funkci vytvorte a naplnte indirect buffer
	*/
	glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT,0,3,0);


	glBindVertexArray(0);
}

void DU01::onKeyPress(SDL_Keycode key, Uint16 mod){
	switch (key) {
	case SDLK_ESCAPE:
		quit();
	}
}






int main(int /*argc*/, char ** /*argv*/)
{
	DU01 app;
	return app.run();
}