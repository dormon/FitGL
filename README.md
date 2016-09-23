# FitGL
*Jednoduchá knihovna pro výuku OpenGL.
*Umí vše potřebné pro záčatek práce s opengl - načítání modelu, textury, gui, matematika.
*Sada krátkých příkladů.

## Knihovny
Assimp - modely
http://www.assimp.org/
FreeImagePlus - Oprázky
http://freeimage.sourceforge.net/
SDL2 - okno, vstupy
https://www.libsdl.org/
GLM - matematika
http://glm.g-truc.net/0.9.8/index.html
CMake - buildovaní
https://cmake.org/
GIT
https://tortoisegit.org/
http://www.syntevo.com/smartgit/
sudo apt-get install git-all

## Zprovoznění
* git clone
* cmake-gui v rootu projektu (tam kde je CMakeLists.txt)
* vyrat zdrojovou složku (FitGL/) a cílovou složku (FitGL/build)
* configure, vybrat oblíbený překladač (Visual Studio, Makefile, ...)
* pokud cmake nenajde knihovny automaticky, je potřeba vyplnit libky a cesty k hlavičkám
* generate
* překlad vygenerovaného projektu