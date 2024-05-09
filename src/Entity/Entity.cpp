#include "Entity.hpp"

Entity::Entity()
{
}

Entity::Entity(vec2f _pos, vec2f _scale, SDL_Texture *_texture, int _uid)
{
	m_pos = _pos;
	m_texture = _texture;
	m_uid = _uid;
	m_current_frame.w = _scale.x;
	m_current_frame.h = _scale.y;
}

void Entity::set_pos(float xpos, float ypos)
{
	m_pos.x = xpos;
	m_pos.y = ypos;
}

void Entity::set_scale(float w, float h)
{
	m_scale.x = w;
	m_scale.y = h;
}

void Entity::set_angle(float _angle)
{
	m_angle = _angle;
}

void Entity::set_life(float _maxLife, float _life)
{
	m_life = Life(_maxLife, _life);
}

void Entity::set_speed(float _speed)
{
	m_speed = _speed;
}

void Entity::change_state(State _state, const char* reason)
{
	std::cout << reason << std::endl;
	m_current_state = _state;
}

void Entity::change_visibility(bool _visible)
{
	m_visible = _visible;
}

void Entity::set_move(bool _moving)
{
	m_moving = _moving;
}

void Entity::input(SDL_Event event)
{
	
}

SDL_Texture* Entity::get_texture()
{
	return m_texture;
}
void Entity::set_texture(SDL_Texture* _texture)
{
	if(_texture == nullptr){
		std::cout << "Texture is null" << std::endl;
		return;
	}
	m_texture = _texture;
}

void Entity::set_renderer(EntityRenderer *_renderer)
{
	m_renderer = _renderer;
}

void Entity::add_sprite_animation(SpriteAnimation _animation)
{
	m_animations.push_back(_animation);
}

void Entity::set_animation(std::string _name)
{
	for (auto const& animation : m_animations) {
		if (animation.name == _name) {
			//mCurrentAnimation = animation;
			return;
		}
	}
}

SDL_Rect Entity::get_current_frame()
{
	return m_current_frame;
}

vec2f Entity::get_pos()
{
	return m_pos;
}

vec2f Entity::get_scale()
{
	return m_scale;
}

float Entity::get_speed()
{
	return m_speed;
}

float Entity::get_angle()
{
	return m_angle;
}

int Entity::get_uid()
{
	return m_uid;
}

bool Entity::is_visible()
{
	return m_visible;
}

bool Entity::is_moving()
{
	return m_moving;
}

bool Entity::is_close_to_pos(vec2f pos, float radius)
{
    return (pos.x + radius < m_pos.x && m_pos.x < pos.x + radius) && 
           (pos.y - radius < m_pos.y && m_pos.y < pos.y + radius);
}

void Entity::hit(float damage, Entity* from)
{
	m_life.add(damage);
}

void Entity::update(double deltaTime)
{
}

void Entity::draw(Resources* resources)
{
}

bool Entity::is_alive()
{
    return m_life.is_alive();
}

float Entity::get_current_life()
{
    return m_life.value;
}
