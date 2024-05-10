#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <map>
#include <string>

#include "../../Resources/Resources.hpp"

struct GUIAssets{
    SDL_Texture* hover;
    
    void load_assets(Resources* resources){
        hover = resources->GetAsset("hover")->GetTexture();
    }
};