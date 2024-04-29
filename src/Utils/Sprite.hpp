#pragma once
#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"

#include "../Resources/Resources.hpp"

//this script will handle all the functionality that a sprite needs
struct SpriteAnimation{
    std::string name;
    std::vector<std::string> frames;

    int current_frame = 0;
    int frameCount = 0;
    //the time that will be used in between the frames
    double frame_timer = 0.8f;
    double m_frame_time = 0;

    //default constructor
    SpriteAnimation()
        :name("default"), frames(std::vector<std::string>()), current_frame(0), frameCount(0), frame_timer(0.8f), m_frame_time(0)
    {}

    SpriteAnimation(std::string name, std::vector<std::string> frames, double frameTimer){
        this->name = name;
        this->frames = frames;
        this->frameCount = frames.size();
        this->frame_timer = frameTimer;
        print();
    }

    void update(double deltaTime){
        m_frame_time += deltaTime;

        //TODO: need to fix this double issue so its possible to modify the frame timer
        if(m_frame_time >= 0.1f){
            m_frame_time = 0;
            current_frame += 1;
        }
        if(current_frame >= frameCount){
            current_frame = 0;
        }
    }
    std::string get_current_frame(){
        return frames[current_frame];
    }

    void print(){
        std::cout << "_______________ANIMATION________________" << std::endl;
        std::cout << "Animation Name : " << name << std::endl;
        std::cout << "Frames : " << std::endl;
        for(auto const& frame : frames){
            std::cout << frame << std::endl;
        }
        std::cout << "Current Frame : " << current_frame << std::endl;
        std::cout << "Frame Count : " << frameCount << std::endl;
        std::cout << "Frame Timer : " << frame_timer << std::endl;
        std::cout << "_______________ANIMATION________________" << std::endl;
    }
};
