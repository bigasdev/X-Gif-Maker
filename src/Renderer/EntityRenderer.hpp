#ifndef _Entity_Renderer_hpp
#define _Entity_Renderer_hpp
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <memory>

class EntityRenderer {
public:
	EntityRenderer(SDL_Renderer *renderer, float _scale);
	~EntityRenderer();

	//void draw_font(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);

	//void draw_pixel(Uint16 p_x, Uint16 p_y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void draw_pixel(float p_x, float p_y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
private:
	SDL_Renderer* m_renderer_ptr;
	float m_game_scale = 1;
};

#endif