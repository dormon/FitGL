#include "pgp2016.h"

struct DrawArraysIndirectCommand {
	GLuint count;
	GLuint instanceCount;
	GLuint first;
	GLuint baseInstance;
};

void PGP2016::du01_initIndirectBuffer() {
	/*****  DU02 - DOPLNIT  *****/
	/*
	- vygenerovani indirect buffer (promenna indirectBuffer)
	- naplneni daty
	- vyuzijte promennych *Vertices, *FirstVertex
	-
	- jsou 4 objekty - koule, podlaha, koberec na stole, stul
	-
	- Workaround pro kompatibilitu s Intel kartama:
	- Jako baseInstance pouzijte cislo objektu (koule-0, zbytek 1,2,3)
	*/
}

void PGP2016::du01_multidraw() {
	glBindVertexArray(vertexArray);
	/*****  DU02 - DOPLNIT  *****/
	/*
	- pripojit indirect buffer
	- jedno volani glMultiDrawArraysIndirect
	*/
}



