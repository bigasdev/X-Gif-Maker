#include "GameScene.hpp"


GameScene::GameScene(App *app, Logger *logger, Cooldown *cooldown, Camera *camera):Scene(app, logger, cooldown, camera)
{

}

void GameScene::load_assets()
{
    
}

void GameScene::init()
{
    m_cd->set_state("init_event", .1f, [&] {	m_logger->log("Starting the GameScene!");});

    //
    load_assets();
}

void GameScene::update(double deltaTime)
{

}

void GameScene::ui()
{

}

void GameScene::draw()
{
	//ui
	GUI::draw([this](){this->ui();});
}

void GameScene::input(SDL_Event event)
{

}

void GameScene::clean()
{

}

