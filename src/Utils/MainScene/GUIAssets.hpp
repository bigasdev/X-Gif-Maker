#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <map>
#include <string>

#include "../../Resources/Resources.hpp"

struct GUIAssets{
    SDL_Texture* cursor;
    SDL_Texture* hover;
    SDL_Texture* select;

    void load_assets(Resources* resources){
        cursor = resources->GetAsset("cursor")->GetTexture();
        select = resources->GetAsset("selected")->GetTexture();
        hover = resources->GetAsset("hover")->GetTexture();
    }
};