#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(Atlas *atlas, int max_particles)
{
    m_atlas_ptr = atlas;
    m_max_particles = max_particles;
    m_current_particles = 0;
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(double delta_time)
{ 
    //delete the particle memory if it's not alive
    if(is_deleting_particle){
        for (auto it = m_particles_vector.begin(); it != m_particles_vector.end();)
        {
            if(!it->alive){
                it = m_particles_vector.erase(it);
            }
            else{
                ++it;
            }
        }
        is_deleting_particle = false;
    }

    for (auto &particle : m_particles_vector)
    {
        if(!particle.alive)continue;

        particle.position.x += particle.speed.x * delta_time;
        particle.position.y += particle.speed.y * delta_time;
        particle.life_time -= delta_time;
        particle.color.a -= particle.opacity_decrease * delta_time;

        if(particle.life_time <= 0){
            particle.alive = false;
            is_deleting_particle = true;
            //delete &particle;
        }
    }
}

void ParticleSystem::draw()
{
    for (auto &particle : m_particles_vector)
    {
        if(!particle.alive)continue;

        m_atlas_ptr->draw_pixel(particle.position.x, particle.position.y, particle.color.r, particle.color.g, particle.color.b, particle.color.a);
    }
}

void ParticleSystem::add_particle(Particle particle)
{
    if(m_particles_vector.size() >= m_max_particles)return;

    Particle test;
    Uint8 current_quadrant = 0;
    if(particle.type == PARTICLE_SQUARE){
        current_quadrant = particle.particles_amount / 4;
    }

    for(int i = 0 ; i < particle.particles_amount; i++){
        Particle p = particle;
        if(particle.type == PARTICLE_SQUARE){
            p.position.x = particle.position.x + (rand() % 20 + (-10));
            p.position.y = particle.position.y + (rand() % 20);

            //vec2f cur_speed = p.speed;


            //p.speed = {cur_speed.x + rand() % 100 + (-100), cur_speed.y + rand() % 100 + (-100)};
        }
        else if(particle.type == PARTICLE_CIRCLE){
            float angle = (rand() % 360) * 3.14159 / 180;
            p.position.x = particle.position.x + cos(angle) * (rand() % 100);
            p.position.y = particle.position.y + sin(angle) * (rand() % 100);
        }

        float curr_life = p.life_time;

        p.opacity_decrease = rand() % 14 + 1;

        p.life_time = rand() % 6 + (curr_life);

        m_particles_vector.push_back(p);
    }
}
