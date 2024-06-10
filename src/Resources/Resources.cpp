#include "Resources.hpp"

std::string mCurrentFolderResources = "";

Resources::Resources(SDL_Renderer* renderer)
{
	std::cout << "Instantied Resources!..." << std::endl;

    mRenderer = renderer;
    LoadAssets();
}

void Resources::LoadFolder(std::string path){
    std::chrono::system_clock::time_point start = 
        std::chrono::system_clock::now();

    auto const& files = Reader::read_file(path);
    int i = 0;
    int max = files.size();

    for(auto const& file : files){
		//SaveTexture(file.c_str());

        //first we get just the file name without the png extension
        std::string fileName = file.substr(file.find_last_of("/") + 1);
        fileName = fileName.substr(0, fileName.find_last_of("."));
        
        CreateAssetData(file.c_str(), fileName.c_str(), mCurrentFolderResources.c_str());

        i += 1;

        //current percentage:
        float percentage = (float)i / (float)max;
        std::string percentageString = std::to_string(percentage * 100);
        std::cout << "Loading: " << percentage * 100 << "%" << std::endl;
	}

    auto end = std::chrono::system_clock::now();


    //check the diff in milisseconds
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}


void Resources::LoadAssets()
{
    std::cout << "Loading the assets!, first we get the folders : \n" << std::endl;
    auto const& folders = Reader::get_folders("res/sprites/");

    mFolders.push_back("sprites");

    for(auto const& folder : folders){
        std::cout << "Loading the folder: " << folder << std::endl;

        //we get the string after the last "/"
        std::string folderName = folder.substr(folder.find_last_of("/") + 1);
        //std::cout << "Folder test name " << folderName << std::endl;

        mFolders.push_back(folderName);

        for(auto const& f : mFolders){
            std::cout << "Folder stored name: " << f << std::endl;
        }

        mCurrentFolderResources = folderName;
        LoadFolder(folder+"/");
    }

    mCurrentFolderResources = "sprites";
    LoadFolder("res/sprites/");

    std::cout << "Finished the assets loading! \n" << std::endl;
}

SDL_Texture *Resources::LoadTexture(const char *file)
{
    SDL_Texture* texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", file);
    texture = IMG_LoadTexture(mRenderer, file);
    if (texture) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loaded %s", file);
    }
    else {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Texture can't be loaded!");
    }
    return texture;
}

SDL_Texture *Resources::SaveTexture(const char *file)
{
    SDL_Texture* texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", file);
    texture = IMG_LoadTexture(mRenderer, file);
    if (texture) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loaded %s", file);
        //adding the texture to the cache
        mTextures.insert(std::pair<const char*, SDL_Texture*>(file, texture));
    }
    else {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Texture can't be loaded!");
    }
    return texture;
}
void Resources::CreateAssetData(const char* file, const char* name, const char* folder)
{
    SDL_Texture* texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Trying to create asset data :  %s, folder : %s", name, folder);
    texture = IMG_LoadTexture(mRenderer, file);
    if (texture) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Created asset data for: %s", file);
        //adding the texture to the cache
        mAssets.insert(std::pair<std::string, AssetData*>(name, new AssetData(name, folder, texture)));
    }
    else {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Asset data couldnt be created");
    }
}

std::vector<AssetData *> Resources::GetAssets(std::string folder)
{
    //we get all the assets of a folder
    std::vector<AssetData*> assets;
    for(auto const& asset : mAssets){
        if(asset.second->GetFolder() == folder){
            assets.push_back(asset.second);
        }
    }
    return assets;
}

AssetData *Resources::GetAsset(std::string name)
{
    //we get the asset by the name
    try{
        
        return mAssets.at(name);
    }catch(const std::out_of_range& e){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Asset %s not found!", name);
        return nullptr;
    }
}

SDL_Texture *Resources::GetTexture(const char *file, bool debug)
{
    auto const& cache = HandleMap(file);
    if (cache) {
        if(debug)SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "IMAGE %s on cache", file);
        return cache;
    }else{
        if(debug)SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Texture not found!");
        return nullptr;
    }
}

SDL_Texture* Resources::HandleMap(const char* file)
{
   //we will get the texture from the map without a loop
    try{
        std::string filePath = "res/sprites/";
        filePath += file;
        return mTextures.at(filePath);
    }catch(const std::out_of_range& e){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Texture %s not found!", file);
        return nullptr;
    }
}


std::vector<std::string> Resources::GetFolders()
{
    return mFolders;
}

std::map<std::string, SDL_Texture *> Resources::GetTextures()
{
    return mTextures;
}

std::vector<std::string> Resources::GetTexturesName()
{
    //create a vector with the keys of the mTextures
    std::vector<std::string> keys;
    for(auto const& texture : mTextures){
        keys.push_back(texture.first.substr(texture.first.find_last_of("/") + 1));
    }
    return keys;
}


