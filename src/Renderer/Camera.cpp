#include "Camera.hpp"

Camera::Camera(Entity* _target)
{
	m_target = _target;

	if (m_target == nullptr)return;

	m_current_pos = m_target->get_pos();
	m_current_pos.x -= m_camera_size.x / 2;
	m_current_pos.y -= m_camera_size.y / 2;
}

void Camera::move(double deltaTime)
{
	if (m_target == nullptr)return;

	//if (!target->isMoving())return;

	m_ref_pos = m_target->get_pos();
	m_ref_pos.x -= m_camera_size.x / 2;
	m_ref_pos.y -= m_camera_size.y / 2;

	float x = m_ref_pos.x - m_current_pos.x;
	float y = m_ref_pos.y - m_current_pos.y;

	//std::cout << "Quick test: " << refPos.x << " " << refPos.y << "Camera : " << x << " " << y << std::endl;

	//currentPos.x = refPos.x - (cameraSize.x / 2);
	//currentPos.y = refPos.y - (cameraSize.y / 2);

	m_current_pos.x = (1 - m_smooth_speed) * m_current_pos.x + m_smooth_speed * m_ref_pos.x;
	m_current_pos.y = (1 - m_smooth_speed) * m_current_pos.y + m_smooth_speed * m_ref_pos.y;
	
	/*
	currentPos.x += (x + smoothSpeed)-cameraSize.x/2;
	currentPos.y += (y + smoothSpeed)-cameraSize.y/2;*/

	//std::cout << "Current position of the camera : " << currentPos.x << "" << currentPos.y << std::endl;
}

void Camera::update_camera_size(vec2f size)
{
	m_camera_size = size;
}

vec2f Camera::get_pos()
{
	return m_current_pos;
}
