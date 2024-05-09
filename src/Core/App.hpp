#pragma once

#include "SDL.h"
#include "../Resources/Resources.hpp"
#include <iostream>
#include "Assert.h"
#include "IniHandler.hpp"
#include "../Renderer/Atlas.hpp"

class Scene;
//global variables
extern bool debug_mode;

class App {

public:

	App();
	~App();

	void init(const char* title, uint32_t xpos, uint32_t ypos, uint32_t width, uint32_t height, bool fullscreen, bool splash_screen);
	void load();
	void handle_events();
	void update(double deltaTime);
	void render();
	void clean();

	//Functionality
	void change_scene(Scene* scene);
	void change_to_next_scene();

	//Util methods
	void change_background_color(vec3f color);

	//Local getters
	vec2f get_window_size();

	//System methods
	float get_fps();
	void set_fps(float fps);
	bool running();

	//Getters
	SDL_Window* get_window();
	SDL_Renderer* get_renderer();
	Resources* get_resources();
	Atlas* get_atlas();
	IniHandler* get_ini_handler();	
	TTF_Font* get_main_font();


private:
	bool m_is_running = false;
	bool m_is_loaded = false;
	bool m_has_splash_screen = true;

	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	Resources* m_resources_ptr = nullptr;
	Atlas* m_atlas_ptr = nullptr;
	IniHandler* m_ini_handler_ptr = nullptr;

	//Local
	vec2f m_window_size = vec2f(0,0);
	vec3f m_window_color = vec3f(26, 25, 50);

	//System
	int m_fps = 0;
};