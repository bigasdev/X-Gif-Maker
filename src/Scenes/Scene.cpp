#include "Scene.hpp"

Scene::Scene(App* app, Logger* logger, Cooldown* cooldown, Camera* camera)
{
	m_app = app;
	m_logger = logger;
	m_cd = cooldown;
	m_camera = camera;
}

void Scene::init()
{
}

void Scene::input(SDL_Event event)
{
}

void Scene::update(double deltaTime)
{
}

void Scene::draw()
{
}

void Scene::clean()
{
}
