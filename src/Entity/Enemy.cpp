#include "Enemy.hpp"

Enemy::Enemy()
{
}

Enemy::Enemy(vec2f _pos, vec2f _scale, SDL_Texture *_texture, int _uid)
{
    m_pos = _pos;
    m_scale = _scale;
    m_texture = _texture;
    m_uid = _uid;

    //random initializers
    m_spawned_timer = Random::get(0, 5);

    m_idle_time = Random::get(1, 9);
    m_walk_time = Random::get(2, 8);
    m_moving_right = Random::get<bool>(0.5);
}

Enemy::~Enemy()
{
}

void Enemy::set_name(std::string name)
{
}

void Enemy::set_state(EnemyState state)
{
}

std::string Enemy::get_name()
{
    return std::string();
}

EnemyState Enemy::get_state()
{
    return m_state;
}

bool Enemy::is_moving_right()
{
    return m_moving_right;
}

void Enemy::update(double deltaTime)
{
}

void Enemy::draw(Resources *resources)
{
    if(m_life.value < m_life.max)
        for(int i = 0; i < m_life.value; i++){
            m_renderer->draw_pixel(m_pos.x + i, m_pos.y + 45, 255, 0, 0, 255);
        }
}

void Enemy::input(SDL_Event event)
{
}
