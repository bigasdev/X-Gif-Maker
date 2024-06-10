#pragma once

#include "../Tools/Reader.hpp"
#include "AssetData.hpp"

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>


class Resources {
public:
	Resources(SDL_Renderer* renderer);

	void LoadFolder(std::string path);
	void LoadAssets();
	SDL_Texture* LoadTexture(const char* file);
	SDL_Texture* SaveTexture(const char* file);
	SDL_Texture* GetTexture(const char* file, bool debug);
	SDL_Texture* HandleMap(const char* file);

	//asset data creation
	void CreateAssetData(const char* file, const char* name, const char* folder);
	std::vector<AssetData*> GetAssets(std::string folder);
	AssetData* GetAsset(std::string name);

	//getters
	std::vector<std::string> GetFolders();

	std::map<std::string, SDL_Texture*> GetTextures();
	std::vector<std::string> GetTexturesName();

private:
	std::map<std::string, SDL_Texture*> mTextures;
	std::map<std::string, AssetData*> mAssets;
	SDL_Renderer* mRenderer;

	std::vector<std::string> mFolders;
};

