//default
#include <list>

#include "App.hpp"
#include "../Utils/Common.hpp"
#include "../Entity/Entity.hpp"
#include "../Resources/Resources.hpp"
#include "SDL_ttf.h"
#include <string>
#include "../Renderer/AppGui.hpp"
#include "../Utils/Math.hpp"
#include "../Renderer/Camera.hpp"
#include "../Tools/Cooldown.hpp"
#include "../Tools/Logger.hpp"
#include "../Scenes/Scenes.hpp"

//global variable
bool debug_mode = false;

//Core functionality
Logger* m_logger = nullptr;
Cooldown* m_cd = nullptr;
Camera* m_camera = nullptr;

//Scenes
Scene* m_current_scene = nullptr;
Scene* m_next_scene = nullptr;

TTF_Font* s_main_font;

//Ideally this class should never be touched
//The important values will be exposed from the other scenes

//ALL the features should be done inside their own scene

App::App()
{
	std::cout << "Instantied App!..." << std::endl;
}

App::~App()
{
}

void App::init(const char* title, uint32_t xpos, uint32_t ypos, uint32_t width, uint32_t height, bool fullscreen, bool splash_screen)
{
	//This can be edited to add more flags to the window, like borderless etc
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	if (fullscreen) {
		window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI| SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	//SDL Initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "App initialized!..." << std::endl;
		m_window = SDL_CreateWindow(title, xpos, ypos, width, height, window_flags);
		int h = 0, w = 0;
		SDL_GetWindowSize(m_window, &h, &w);
		//ensure the window cant be so small
		SDL_SetWindowMinimumSize(m_window, 1000, 400);
		m_window_size = vec2f(h, w);

		//This is true if the windows was created
		if (m_window) {
			std::cout << "Window created!..." << std::endl;
		}

		int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

		IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			fprintf(stderr, "SDL could not initialize\n");

		}
		if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		{
			fprintf(stderr, "SDL_image could not initialize\n");
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);

		//This is true if the renderer was created
		if (m_renderer) {
			SDL_SetRenderDrawColor(m_renderer, 50, 40, 0, 255);
			std::cout << "Renderer created!..." << std::endl;
			m_has_splash_screen = splash_screen;

			//Initializing the GUI
			TTF_Init();
			GUI::setup(m_window, m_renderer);


			m_atlas_ptr = new Atlas(m_renderer, 1);

			s_main_font = TTF_OpenFont("res/font/pixolleta.ttf", 10);
		}
		m_is_running = true;
		}
	else {
		m_is_running = false;
	}
}

void App::load()
{
	if(m_is_loaded) return;
	//loading resources
	{
		m_resources_ptr = new Resources(m_renderer);
		m_cd = new Cooldown();
		m_camera = new Camera(nullptr, m_window_size);

		m_logger = new Logger(m_resources_ptr->GetAsset("logger")->GetTexture());
		m_logger->init(m_atlas_ptr, s_main_font, vec2f(46, 14));

		m_ini_handler_ptr = new IniHandler();

			//Starting scenes
		if(m_has_splash_screen){
			m_current_scene = new IntroScene(this, m_logger, m_cd, m_camera);
		}else{
			m_current_scene = new MainScene(this, m_logger, m_cd, m_camera);
		}
		m_next_scene = new MainScene(this, m_logger, m_cd, m_camera);
			

		m_is_loaded = true;
		m_current_scene->init();
	}
}

//This function will be the core of all the events that some scripts can require
//And will be responsible for listening to the inputs
void App::handle_events()
{
	if(!m_is_loaded) return;

	SDL_Event event;
	SDL_PollEvent(&event);

	GUI::event(event);
	m_current_scene->input(event);

	switch (event.type) {
	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
			//updating window size
			{
				int h = 0, w = 0;
				SDL_GetWindowSize(m_window, &h, &w);
				m_window_size.x = h;
				m_window_size.y = w;
			}
		}
		break;

	case SDL_QUIT:
		m_is_running = false;
		break;

	case SDL_KEYDOWN:
		//moving gathering input
		//mCurrentScene->Input(event.key.keysym.scancode);
		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_D:
			debug_mode = !debug_mode;
			m_logger->log("Debug mode: " + std::to_string(debug_mode));
			break;
		case SDL_SCANCODE_A:

			break;
		case SDL_SCANCODE_W:

			break;
		case SDL_SCANCODE_S:
			//m_logger->log("You pressed S");
			
			break;
		case SDL_SCANCODE_E:
			//m_logger->log("Testing the logger");
			//ChangeToNextScene();
			break;
		case SDL_SCANCODE_ESCAPE:
			//m_is_running = false;
			break;
		}
		break;

	case SDL_KEYUP:
		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_D:

			break;
		case SDL_SCANCODE_A:

			break;
		case SDL_SCANCODE_W:

			break;
		case SDL_SCANCODE_S:

			break;
		}
		break;

	default:
		break;
	}
}


void App::update(double deltaTime)
{
	if(!m_is_loaded) return;

	m_cd->update(deltaTime);
	m_logger->update(deltaTime);

	if (m_current_scene != nullptr)m_current_scene->update(deltaTime);
}

//The core sdl renderer, this will encapusalate the scene rendering so there is no need to clear and call the color inside the scenes
void App::render()
{
	SDL_SetRenderDrawColor(m_renderer, m_window_color.x, m_window_color.y, m_window_color.z, 255);
	SDL_RenderClear(m_renderer);
	//rendering loop here
	if(m_is_loaded){
		//scene drawing
		if (m_current_scene != nullptr)m_current_scene->draw();

		//logger rendering
		m_logger->draw();
	}else{
		m_atlas_ptr->draw((m_window_size.x - 70)/2, (m_window_size.y - 50)/2, "Loading...", s_main_font, {255,255,255,255});

	}
	SDL_RenderPresent(m_renderer);
}

void App::clean()
{
	GUI::clean();
	m_current_scene->clean();
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	TTF_Quit();
	SDL_Quit();
	std::cout << "Game Cleaned!..." << std::endl;
}

void App::change_scene(Scene* scene)
{
	m_current_scene->clean();
	m_current_scene = scene;
	m_current_scene->init();
}

void App::change_to_next_scene()
{
	m_current_scene->clean();
	m_current_scene = m_next_scene;
	m_current_scene->init();
}

void App::change_background_color(vec3f color)
{
	m_window_color = color;
}

vec2f App::get_window_size()
{
	return m_window_size;
}

TTF_Font *App::get_main_font()
{
    return s_main_font;
}

float App::get_fps()
{
    return m_fps;
}

void App::set_fps(float fps)
{
	m_fps = Math::clamp(0, fps, 60);
}

SDL_Window* App::get_window()
{
	return m_window;
}

SDL_Renderer* App::get_renderer()
{
	return m_renderer;
}

Resources* App::get_resources()
{
	return m_resources_ptr;
}

Atlas* App::get_atlas()
{
	return m_atlas_ptr;
}

IniHandler *App::get_ini_handler()
{
    return m_ini_handler_ptr;
}

bool App::running()
{
	return m_is_running;
}
