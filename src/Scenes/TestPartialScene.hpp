#pragma once

#include "PartialScene.hpp"

class TestPartialScene : public PartialScene
{
public:
    TestPartialScene(App *app, Logger *logger, Cooldown *cooldown, Camera *camera);

    void init() override;
    void input(SDL_Event event) override;
    void update(double deltaTime) override;
    void draw() override;
    void clean() override;

    void add_string(std::string* str);

private:
    // Add your variables here

    std::string* m_test_string;
};