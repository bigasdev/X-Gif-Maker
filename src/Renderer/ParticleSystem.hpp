#ifndef _PARTICLE_SYSTEM_HPP_
#define _PARTICLE_SYSTEM_HPP_

#pragma once

#include "../Renderer/Atlas.hpp"
#include "../Utils/Math.hpp"
#include <vector>
#include <math.h>

#include "SDL.h"

enum ParticleType{
    PARTICLE_SQUARE,
    PARTICLE_CIRCLE
};

struct Particle{
    ParticleType type;

    Uint32 particles_amount;

    vec2f position;
    vec2f speed;
    float life_time;
    float opacity_decrease;
    SDL_Color color;

    bool alive;
};


//System that will be used to create and manage particles
//right now it will work with creating a class for each kind of particles/
//TODO: make it ecs based
class ParticleSystem{
public:
    ParticleSystem(Atlas* atlas, int max_particles);
    ~ParticleSystem();

    void update(double delta_time);
    void draw();

    void add_particle(Particle particle);

private:
    Atlas* m_atlas_ptr;

    int m_max_particles;
    int m_current_particles;

    bool is_deleting_particle = false;

    std::vector<Particle> m_particles_vector;
};

#endif