#pragma once

#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>

#include <string>
/* Useful 'macros' to help with imgui */

using namespace ImGui;

void label(std::string const&text, int x=0, int y=0, int width = 100, int height = 20);