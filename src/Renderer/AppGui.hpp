#pragma once
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_sdl.h"
#include "../ImGui/imgui_impl_sdlrenderer.h"
#include <functional>
#include <stdio.h>
#include <SDL.h>

namespace GUI {
	void setup(SDL_Window *window, SDL_Renderer *renderer);
	void event(SDL_Event event);
	void draw(std::function<void()> function);
	void clean();

	//util functions
	void set_focus();
}