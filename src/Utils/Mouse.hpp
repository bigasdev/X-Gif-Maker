#pragma once
#ifndef MOUSE_HPP
#define MOUSE_HPP

#include "SDL.h"
#include "Common.hpp"
#include <string>
#include <map>
namespace Mouse {

	bool is_at_area(Area pArea);
	vec2f get_mouse_pos();

	class MouseCursor{
		SDL_Rect rect;
		SDL_Rect point;

		public:
			MouseCursor(SDL_Rect rect = {0, 0, 16, 16}, SDL_Rect point = {0,0,1,1}) {
				//SDL_ShowCursor(SDL_DISABLE);
				this->rect = rect;
				this->point = point;
			}
			void disable_cursor(){
				SDL_ShowCursor(true);
			}
			void update(){
				vec2f mousePos = get_mouse_pos();
				rect.x = mousePos.x;
				rect.y = mousePos.y;
				point.x = mousePos.x;
				point.y = mousePos.y;
			}
			void draw(SDL_Texture* tex, SDL_Renderer* ren){
				//SDL_ShowCursor(SDL_DISABLE);
				SDL_RenderCopy(ren, tex, NULL, &rect);
			}
	};
};

#endif