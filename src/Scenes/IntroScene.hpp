#pragma once
#include "Scene.hpp"

class IntroScene : public Scene {
public:
	IntroScene(App* app, Logger* logger, Cooldown* cooldown, Camera* camera);

	void init();
	void update(double deltaTime);
	void draw();
	void input(SDL_Event event);
	void clean();
	void load_assets();
};