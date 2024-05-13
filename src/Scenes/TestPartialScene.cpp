#include "TestPartialScene.hpp"

TestPartialScene::TestPartialScene(App *app, Logger *logger, Cooldown *cooldown, Camera *camera) : PartialScene(app, logger, cooldown, camera)
{
}

void TestPartialScene::init()
{
    m_cd->set_state("test_partial_init_event", .3f, [&] { 
        m_logger->log("Starting the TestPartialScene!");
        std::cout << m_test_string->c_str() << std::endl;
     });
}

void TestPartialScene::input(SDL_Event event)
{
}

void TestPartialScene::update(double deltaTime)
{
    
}

void TestPartialScene::draw()
{
    if(m_test_string == nullptr) return;

    m_atlas->draw(300, 300, m_test_string->c_str(), m_app->get_main_font(), {255,255,255,125});
}

void TestPartialScene::clean()
{
}

void TestPartialScene::add_string(std::string *str)
{
    m_test_string = str;
}
