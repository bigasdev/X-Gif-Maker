#ifndef _MAINSCENE_HPP
#define _MAINSCENE_HPP
#include "Scene.hpp"

#include "../Core/DataLoader.hpp"
#include "../Entity/FileEntity.hpp"

#include <ctime> 
#include <thread>
#include <atomic>
#include <filesystem>

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
	std::vector<FileEntity> m_files;
};

#endif
