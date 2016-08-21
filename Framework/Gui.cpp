#include "Gui.h"

using namespace ImGui;
void label(std::string const & text, int x, int y, int width, int height){
	Begin("", 0, ImVec2(width, height), 0, 
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
	SetWindowPos(ImVec2(x,y));
	Text(text.c_str());

	End();
}
