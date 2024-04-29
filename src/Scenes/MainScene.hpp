#ifndef _MAINSCENE_HPP
#define _MAINSCENE_HPP
#include "Scene.hpp"

#include "../Core/DataLoader.hpp"

#include <ctime> 
#include <thread>
#include <atomic>

class MainScene : public Scene {
public:
	MainScene(App* app, Logger* logger, Cooldown* cooldown, Camera* camera);

	void init();
	void update(double deltaTime);
	void draw();
	void input(SDL_Event event);
	void clean();
	void load_assets();

	void ui();

private:
	
};

#endif
