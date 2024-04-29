#include "EntityRenderer.hpp"

EntityRenderer::EntityRenderer(SDL_Renderer *renderer, float _scale)
{
    m_game_scale = _scale;
    m_renderer_ptr = renderer;
}

EntityRenderer::~EntityRenderer()
{
}

void EntityRenderer::draw_pixel(float p_x, float p_y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(m_renderer_ptr, r, g, b, a);
	SDL_SetRenderDrawBlendMode(m_renderer_ptr, SDL_BLENDMODE_ADD);
	SDL_RenderDrawPoint(m_renderer_ptr, p_x, p_y);
	SDL_RenderDrawPoint(m_renderer_ptr, p_x+1, p_y);
	SDL_RenderDrawPoint(m_renderer_ptr, p_x+1, p_y+1);
	SDL_RenderDrawPoint(m_renderer_ptr, p_x, p_y+1);
}
