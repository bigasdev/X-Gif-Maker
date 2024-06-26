#pragma once

#include "../Core/App.hpp"
#include "../Core/Assert.h"
#include "../Core/IniHandler.hpp"
#include "../Renderer/AppGui.hpp"
#include "../Renderer/Atlas.hpp"
#include "../Renderer/Camera.hpp"
#include "../Renderer/AssetsObject.hpp"
#include "../Resources/Resources.hpp"
#include "../Entity/Entity.hpp"
#include "../Tools/Cooldown.hpp"
#include "../Tools/Logger.hpp"
#include "../Utils/Mouse.hpp"
#include "PartialScene.hpp"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <vector>
#include <string>
#include <math.h>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <memory>

//the scene class, we'll use this to create the behaviours of our app.
class Scene {
public:
	Scene(App* app, Logger* logger, Cooldown* cooldown, Camera* camera);

	virtual void init();
	virtual void input(SDL_Event event);
	virtual void update(double deltaTime);
	virtual void draw();
	virtual void clean();

protected:
	App* m_app = nullptr;

	//tools
	Logger* m_logger = nullptr;
	Cooldown* m_cd = nullptr;
	Camera* m_camera = nullptr;
	Resources* m_resources = nullptr;
	IniHandler* m_ini_handler = nullptr;
	Atlas* m_atlas = nullptr;

	std::vector<std::unique_ptr<PartialScene>> m_partial_scenes;
};