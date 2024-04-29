#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <list>
#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"
#include "../Entity/Entity.hpp"
#include "../Renderer/Atlas.hpp"
#include "../Resources/Resources.hpp"
#include "../Utils/Math.hpp"

struct LogData{
public:
	float y;std::string text;

	LogData(std::string text):y(0), text(text){};
};

class Logger {
public:
	Logger(SDL_Texture* logger, float logSpeed = 3.5f, bool enabled = true);

	void init(Atlas* atlas, TTF_Font* font, vec2f scale);

	void log(std::string text);
	void update(float deltaTime);
	void draw();

private:
	//member variables
	vec2f m_scale = vec2f(0, 0);
	float m_log_speed = 3.5f;
	bool m_enabled = true;
	std::list<std::unique_ptr<LogData>> m_logs;

	//pointers
	SDL_Texture* m_logger = nullptr;
	TTF_Font* m_font = nullptr;
	Atlas* m_atlas = nullptr;
};