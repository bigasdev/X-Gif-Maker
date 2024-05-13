#include "PartialScene.hpp"

PartialScene::PartialScene(App *app, Logger *logger, Cooldown *cooldown, Camera *camera)
{
    m_app = app;
	m_logger = logger;
	m_cd = cooldown;
	m_camera = camera;

	m_resources = app->get_resources();
	m_ini_handler = app->get_ini_handler();
	m_atlas = app->get_atlas();
}

void PartialScene::init()
{
    m_cd->set_state("partial_init_event", .2f, [&] {	m_logger->log("Starting the PartialScene!");});
}

void PartialScene::input(SDL_Event event)
{
}

void PartialScene::update(double deltaTime)
{
}

void PartialScene::draw()
{
}

void PartialScene::clean()
{
}
