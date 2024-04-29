#include "Atlas.hpp"

Atlas::Atlas(SDL_Renderer* renderer, float _scale)
{
	std::cout << "Instantiated atlas!..." << std::endl;
	m_game_scale = _scale;
	m_renderer_ptr = renderer;
}

Atlas::~Atlas()
{
}

//the simple draw function, its used to show a sprite in the screen
void Atlas::draw(SDL_Texture* texture, vec2f scale, double size, double xpos, double ypos, bool flip)
{

	SDL_Rect dst;
	dst.x = (xpos + (scale.x * size - scale.x * size) / 2);
	dst.y =	(ypos + (scale.y * size - scale.y * size) / 2);
	dst.w = scale.x * size;
	dst.h = scale.y * size;

	/*SDL_Rect dst;
	dst.w = scale.x*size;
	dst.h = scale.y*size;
	dst.x = xpos + (dst.w * size - dst.w * size * dst.x) / 2;
	dst.y = ypos + (dst.h * size - dst.h * size * dst.y) / 2;*/

	auto flip_state = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	SDL_RenderCopyEx(m_renderer_ptr, texture, NULL, &dst, 0, 0, flip_state);
}
void Atlas::draw(Entity* p_entity, Camera* p_camera) {
	if (!p_entity->is_visible())return;

	SDL_Rect src;
	src.x = p_entity->get_current_frame().x;
	src.y = p_entity->get_current_frame().y;
	src.w = p_entity->get_current_frame().w * m_game_scale;
	src.h = p_entity->get_current_frame().h * m_game_scale;

	SDL_Rect dst;
	dst.x = (p_entity->get_pos().x + (p_entity->get_current_frame().w * m_game_scale - p_entity->get_current_frame().w * m_game_scale * p_entity->get_scale().x) / 2) - p_camera->get_pos().x;
	dst.y = (p_entity->get_pos().y + (p_entity->get_current_frame().h * m_game_scale - p_entity->get_current_frame().h * m_game_scale * p_entity->get_scale().y) / 2) - p_camera->get_pos().y;
	dst.w = p_entity->get_current_frame().w * p_entity->get_scale().x * m_game_scale;
	dst.h = p_entity->get_current_frame().h * p_entity->get_scale().y * m_game_scale;

	//std::cout << "checks src" << "x " << src.x << " y " << src.y << " w " << src.w << " h " << src.h << std::endl; 
	//std::cout << "checks" << "x " << dst.x << " y " << dst.y << " w " << dst.w << " h " << dst.h << std::endl; 

	//std::cout << "just testing? " << p_entity->getTexture() << "renderer" << rendererptr << "size : " << dst.h << std::endl;

	SDL_RenderCopyEx(m_renderer_ptr, p_entity->get_texture(), &src, &dst, p_entity->get_angle(), 0, SDL_FLIP_NONE);
}
void Atlas::draw(std::unique_ptr<Entity> p_entity, Camera* p_camera) {
	if (!p_entity->is_visible())return;

	SDL_Rect src;
	src.x = p_entity->get_current_frame().x;
	src.y = p_entity->get_current_frame().y;
	src.w = p_entity->get_current_frame().w * m_game_scale;
	src.h = p_entity->get_current_frame().h * m_game_scale;

	SDL_Rect dst;
	dst.x = (p_entity->get_pos().x + (p_entity->get_current_frame().w * m_game_scale - p_entity->get_current_frame().w * m_game_scale * p_entity->get_scale().x) / 2) - p_camera->get_pos().x;
	dst.y = (p_entity->get_pos().y + (p_entity->get_current_frame().h * m_game_scale - p_entity->get_current_frame().h * m_game_scale * p_entity->get_scale().y) / 2) - p_camera->get_pos().y;
	dst.w = p_entity->get_current_frame().w * p_entity->get_scale().x * m_game_scale;
	dst.h = p_entity->get_current_frame().h * p_entity->get_scale().y * m_game_scale;

	//std::cout << "checks src" << "x " << src.x << " y " << src.y << " w " << src.w << " h " << src.h << std::endl; 
	//std::cout << "checks" << "x " << dst.x << " y " << dst.y << " w " << dst.w << " h " << dst.h << std::endl; 

	//std::cout << "just testing? " << p_entity->getTexture() << "renderer" << rendererptr << "size : " << dst.h << std::endl;

	SDL_RenderCopyEx(m_renderer_ptr, p_entity->get_texture(), &src, &dst, p_entity->get_angle(), 0, SDL_FLIP_NONE);
}

void Atlas::draw(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
	SDL_Texture* message = SDL_CreateTextureFromSurface(m_renderer_ptr, surfaceMessage);

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = surfaceMessage->w;
	src.h = surfaceMessage->h;

	SDL_Rect dst;
	dst.x = p_x;
	dst.y = p_y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_FreeSurface(surfaceMessage);
	SDL_RenderCopyEx(m_renderer_ptr, message, &src, &dst, 0, 0, SDL_FLIP_NONE);
	SDL_DestroyTexture(message);
}

SDL_Renderer *Atlas::get_renderer()
{
    return m_renderer_ptr;
}

void Atlas::draw_pixel(float p_x, float p_y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(m_renderer_ptr, r, g, b, a);
	SDL_SetRenderDrawBlendMode(m_renderer_ptr, SDL_BLENDMODE_ADD);
	SDL_RenderDrawPoint(m_renderer_ptr, p_x, p_y);
	SDL_RenderDrawPoint(m_renderer_ptr, p_x+1, p_y);
	SDL_RenderDrawPoint(m_renderer_ptr, p_x+1, p_y+1);
	SDL_RenderDrawPoint(m_renderer_ptr, p_x, p_y+1);
}
