#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <string>

//the class that will be used to store the data of the assets
//it will consist with the folder so we can reference it with the selected folder

class AssetData{
public:
    AssetData(std::string name, std::string folder, SDL_Texture* texture);
    ~AssetData();

    std::string GetName();
    std::string GetFolder();
    SDL_Texture* GetTexture();
    
private:
    std::string mName;
    std::string mFolder;
    SDL_Texture* mTexture;
};  