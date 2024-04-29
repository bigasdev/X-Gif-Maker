#pragma once
#include "SDL.h"
#include "../Utils/Common.hpp"
#include "SDL_image.h"
#include "../Utils/Sprite.hpp"
#include "../Resources/Resources.hpp"
#include "../Renderer/EntityRenderer.hpp"

#include "../Utils/random.hpp"

using Random = effolkronium::random_static;

#include <vector>
#include <string>

//main class for the entities, in our app everything that will be displayed and have interaction is an entity
class Entity {
public:
	//constructors, so a entity might not be able to access app if we want to.
	Entity();
	Entity(vec2f _pos, vec2f _scale, SDL_Texture* _texture, int _uid);

	//setters
	void set_pos(float xpos, float ypos);
	void set_scale(float w, float h);
	void set_angle(float _angle);
	void set_life(float _maxLife, float _life);
	void set_speed(float _speed);
	void set_texture(SDL_Texture* _texture);
	void set_renderer(EntityRenderer* _renderer);

	//animation stuff
	void add_sprite_animation(SpriteAnimation _animation);
	void set_animation(std::string _name);

	void change_state(State _state, const char* reason);
	void change_visibility(bool _visible);
	void set_move(bool _moving);

	//input stuff
	void input(SDL_Event event);

	//getters
	SDL_Texture* get_texture();
	SDL_Rect get_current_frame();
	vec2f get_pos();
	vec2f get_scale();
	float get_speed();
	float get_angle();
	int get_uid();
	bool is_visible();
	bool is_moving();
	
	//utils
	bool is_close_to_pos(vec2f pos, float radius);

	//functionality methods
	void hit(float damage, Entity* from);
	void update(double deltaTime);
	void draw(Resources* resources);
	bool is_alive();
	float get_current_life();

	//a default spriteanimation 
	//SpriteAnimation mCurrentAnimation = SpriteAnimation("default", std::vector<std::string>{"default", "default1"}, nullptr);

protected:
	//global variables that every entity should be able to call

	//controller variables
	int m_uid;

	//draw/pos stuff
	EntityRenderer* m_renderer;
	vec2f m_pos;
	float m_angle = 0;
	vec2f m_scale = vec2f(1, 1);
	SDL_Rect m_current_frame = {0,0,0,0};
	SDL_Texture* m_texture;
	bool m_visible = true;
	bool m_moving = false;
	std::vector<SpriteAnimation> m_animations;
	//SpriteAnimation mCurrentAnimation;

	//parameters
	Life m_life = Life(100, 100);
	State m_current_state = UPDATE;
	float m_speed = 1;
};