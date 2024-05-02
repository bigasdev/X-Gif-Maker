#pragma once
#include <SDL.h>
#include "../Utils/Common.hpp"
#include "../Entity/Entity.hpp"

class Camera {
public:
	Camera(Entity* _target, vec2f _screen_size);

	void move(double deltaTime);
	void update_camera_size(vec2f size);
	void set_entity(Entity* _target) { m_target = _target; }
	vec2f get_pos();
private:
	vec2f m_camera_size = vec2f(1024, 768);
	vec2f m_offset = vec2f(0,0);
	float m_smooth_speed = .01f;

	//positions
	vec2f m_current_pos;
	vec2f m_ref_pos;

	Entity* m_target = nullptr;
};