#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "../Entity/Entity.hpp"
#include "../Renderer/Camera.hpp"
#include <iostream>
#include <memory>

class Atlas {
public:
	Atlas(SDL_Renderer *renderer, float _scale);
	~Atlas();

	void draw(SDL_Texture* texture, vec2f scale, double size, double xpos, double ypos, bool flip = false);
	void draw(Entity* entity, Camera* camera);
	void draw(std::unique_ptr<Entity> entity, Camera* camera);
	void draw(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
	void draw_from_sheet(Entity* entity, Camera* camera);

	//getters
	SDL_Renderer* get_renderer();

	//void draw_pixel(Uint16 p_x, Uint16 p_y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void draw_pixel(float p_x, float p_y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
private:
	SDL_Renderer* m_renderer_ptr;
	float m_game_scale = 1;
};