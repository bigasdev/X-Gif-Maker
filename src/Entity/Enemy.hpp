#ifndef _ENEMY_HPP_
#define _ENEMY_HPP_

#include "Entity.hpp"

enum EnemyState{
    ENEMY_IDLE,
    ENEMY_WALKING,
    ENEMY_RUNNING,
    ENEMY_ATTACKING,
    ENEMY_CREATING
};

class Enemy : public Entity{
public:
    Enemy();
    Enemy(vec2f _pos, vec2f _scale, SDL_Texture* _texture, int _uid);
    ~Enemy();

    void set_name(std::string name);
    void set_state(EnemyState state);
    std::string get_name();
    EnemyState get_state();

    bool is_moving_right();

    void update(double deltaTime);
    void draw(Resources* resources);
    void input(SDL_Event event);

private:
    std::string m_name;

    EnemyState m_state = ENEMY_CREATING;

    float m_spawned_timer = 0;
    float m_idle_timer = 0;
    float m_walk_timer = 0;
    float m_speed = 15;

    float m_idle_time = 5;
    float m_walk_time = 5;

    //booleans
    bool m_moving_right = true;
};

#endif
