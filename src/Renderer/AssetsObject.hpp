#pragma once
#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"

struct AssetObject{
    SDL_Texture* texture;
    std::string layer;
    int x, y;

    AssetObject(SDL_Texture* texture, std::string layer, int x, int y){
        this->texture = texture;
        this->layer = layer;
        this->x = x;
        this->y = y;
    }
};