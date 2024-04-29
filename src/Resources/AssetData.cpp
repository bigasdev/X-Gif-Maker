#include "AssetData.hpp"

AssetData::AssetData(std::string name, std::string folder, SDL_Texture *texture)
{
    mName = name;
    mFolder = folder;
    mTexture = texture;
}

AssetData::~AssetData()
{

}

std::string AssetData::GetName()
{
    return mName;
}

std::string AssetData::GetFolder()
{
    return mFolder;
}

SDL_Texture *AssetData::GetTexture()
{
    return mTexture;
}
