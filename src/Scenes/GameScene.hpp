#ifndef _GAMESCENE_HPP
#define _GAMESCENE_HPP
#include "Scene.hpp"

#include <ctime> 

class GameScene : public Scene {
public:
	GameScene(App* app, Logger* logger, Cooldown* cooldown, Camera* camera);

	void init();
	void update(double deltaTime);
	void ui();
	void draw();
	void input(SDL_Event event);
	void clean();
	void load_assets();


private:
	
};

#endif
