#include "GameScene.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>

SDL_Texture* m_bg_tx_game = nullptr;

std::vector<std::string> m_files;

GameScene::GameScene(App *app, Logger *logger, Cooldown *cooldown, Camera *camera):Scene(app, logger, cooldown, camera)
{

}

void GameScene::load_assets()
{
    m_bg_tx_game = m_resources->GetAsset("main_scene")->GetTexture();
    
}

void GameScene::init()
{
    m_cd->set_state("init_event", .1f, [&] {	m_logger->log("Starting the GameScene!");});

    //
    load_assets();
}

void GameScene::update(double deltaTime)
{

}

void GameScene::ui()
{

}

void convert(){
    std::string batchFilePath = "convert_images.bat";

    std::ofstream batchFile(batchFilePath);
    if (!batchFile.is_open()) {
        std::cerr << "Failed to create/open batch file." << std::endl;
    }

    batchFile << "@echo off" << std::endl;
    for (size_t i = 0; i < m_files.size(); ++i) {
        batchFile << "copy \"" << m_files[i] << "\" ." << std::endl;
        batchFile << "ren \"" << m_files[i].substr(m_files[i].find_last_of("\\") + 1) << "\" \"image" << i + 1 << ".png\"" << std::endl;
    }
    batchFile << "ffmpeg -framerate 1 -i image%%d.png -vf \"pad=800:600:(ow-iw)/2:(oh-ih)/2\" -t 5 output.gif" << std::endl;
    for (size_t i = 0; i < m_files.size(); ++i) {
        batchFile << "del image" << i + 1 << ".png" << std::endl;
    }

    batchFile.close();

    //std::remove("convert_images.bat");

    int result = system(batchFilePath.c_str());
    if (result != 0) {
        std::cerr << "Command execution failed." << std::endl;
    }
}

void GameScene::draw()
{
	//ui
	GUI::draw([this](){this->ui();});

    m_atlas->draw(m_bg_tx_game, m_app->get_window_size(), 1, 0, 0, false);

    for(int i = 0; i < m_files.size(); i++){
        m_atlas->draw(50, 130 + i * 10 , m_files[i].c_str(), m_app->get_main_font(), {255,255,255,255} );
    }
}

void GameScene::input(SDL_Event event)
{
	//partial scenes
	for(auto& scene : m_partial_scenes){
		scene->input(event);
	}

	switch (event.type) {
		case SDL_DROPFILE:
            m_files.push_back(event.drop.file);
			std::cout << "File dropped on window: " << event.drop.file << std::endl;
			//convert_file();
		break;

		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:

				break;
			}

		case SDL_MOUSEBUTTONUP:
			if(event.type == SDL_MOUSEBUTTONDOWN)return;
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:			

				break;
			}

		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_D:

				break;
				case SDL_SCANCODE_F:
					convert();
					break;
				case SDL_SCANCODE_E:
					m_files.clear();

					/*if(m_file_path != ""){
						convert_file(m_file_path);
					}*/
				break;
				case SDL_SCANCODE_A:
					
				break;
				case SDL_SCANCODE_T:
					
				break;
				case SDL_SCANCODE_RETURN:
					
				break;
				case SDL_SCANCODE_ESCAPE:
					
				break;
				case SDL_SCANCODE_TAB:
					
					
				break;
			}
		break;
		case SDL_KEYUP:
			switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_D:
				break;
				case SDL_SCANCODE_RETURN:
					
				break;
				case SDL_SCANCODE_TAB:
					
				break;
			}
		break;
	}
}

void GameScene::clean()
{

}

