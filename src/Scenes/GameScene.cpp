#include "GameScene.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "../Utils/Vec.hpp"
#include "../Utils/Math.hpp"
#include "ConvertionHandler.hpp"

SDL_Texture* m_bg_tx_game = nullptr;

std::vector<std::string> m_files;

int m_current_file_idx = 0;

struct VideoFrame{
	std::string m_file_path;
	SDL_Texture* m_texture;
	int frame_start;
	int frame_end;
};

Timeline mySequence;
std::vector<VideoFrame> m_video_frames;

int m_current_frame = 0;
float timer = 0.0f;
float m_frame_time = 1.0f;
int m_max_frames = 10;

bool m_is_playing = false;

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

	//
	for(int i = 0; i < sizeof(SequencerItemTypeNames) / sizeof(char*); i++){
		SequencerItemTypeNames[i] = "None";
	}
	//

   	mySequence.mFrameMin = 0;
   	mySequence.mFrameMax = 10;
}


void GameScene::update(double deltaTime)
{
	if(m_files.size() <= 0)return;

	for(int i = 0; i < m_video_frames.size(); i++){
		int* x;
		int* y;
		mySequence.Get(i, &x, &y, nullptr, nullptr);

		m_video_frames[i].frame_start = *x;
		m_video_frames[i].frame_end = *y;
	}

	if(!m_is_playing)return;

	timer += 1*deltaTime;
	if(timer >= m_frame_time){
		timer = 0.0f;
		m_current_frame++;
		if(m_current_frame >= m_max_frames){
			m_current_frame = 0;
		}
	}
}

void GameScene::ui()
{
	{
		// let's create the sequencer
        static int selectedEntry = -1;
        static int firstFrame = 0;
        static bool expanded = true;
        static int currentFrame = 0;

        //Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
		ImGui::SetNextWindowPos(ImVec2(0, 210));
		ImGui::SetNextWindowSize(ImVec2(550, 170));
		if (ImGui::Begin("Timeline", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
			ImGui::PushItemWidth(130);
			ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
			ImGui::SameLine();
			ImGui::InputInt("Frame ", &currentFrame);
			ImGui::SameLine();
			ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
			ImGui::PopItemWidth();
			// Inside the window, you can place your UI elements or other content
			// For example:
			Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
			
			// End the window
			ImGui::End();
		}
		ImGui::SetNextWindowPos(ImVec2(552, 210));
		ImGui::SetNextWindowSize(ImVec2(254, 170));
		if(ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)){
			ImGui::End();
		}
	}
}

//the main method for the image to gif feature
void convert(){
    std::string batchFilePath = "convert_images.bat";

    std::ofstream batchFile(batchFilePath);
    if (!batchFile.is_open()) {
        std::cerr << "Failed to create/open batch file." << std::endl;
    }

	//first we try to create the files txt
	{
		std::ofstream outfile("files.txt"); // Open file for writing

		// Check if the file is successfully opened
		if (!outfile.is_open()) {
			std::cerr << "Error opening file for writing!" << std::endl;
			return;
		}

		for(int i = 0; i < mySequence.myItems.size(); i++){
			int *x;
			int *y;
			int sum;

			mySequence.Get(i, &x, &y, nullptr, nullptr);
			sum = *y-*x;

			std::cout << *x << " " << *y << " " << sum << std::endl;

			outfile << "file image" << i+1 << ".jpeg" << std::endl;
    		outfile << "outpoint " << sum << std::endl;
		}

		// Close the file
		outfile.close();
	}

    batchFile << "@echo off" << std::endl;
    for (size_t i = 0; i < m_files.size(); ++i) {
        batchFile << "copy \"" << m_files[i] << "\" ." << std::endl;
        batchFile << "ren \"" << m_files[i].substr(m_files[i].find_last_of("\\") + 1) << "\" \"image" << i + 1 << ".jpeg\"" << std::endl;
    }
    //batchFile << "ffmpeg -i image%%d.jpeg -vf \"pad=180:100:(ow-iw)/2:(oh-ih)/2\" -t 5 output.gif" << std::endl;
    batchFile << "ffmpeg -f concat -i files.txt -vf \"pad=1600:1200:(ow-iw)/2:(oh-ih)/2\" -t 15 output.mp4" << std::endl;
    for (size_t i = 0; i < m_files.size(); ++i) {
        batchFile << "del image" << i + 1 << ".jpeg" << std::endl;
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

	if(!m_is_playing)return;

	auto condition = [&](const VideoFrame& vf){
		return m_current_frame >= vf.frame_start && m_current_frame <= vf.frame_end;
	};

	auto frame = Vec::find_vec_element<VideoFrame>(m_video_frames, condition);

	if(frame != nullptr){
		m_atlas->draw(frame->m_texture, {256,192}, 1, Math::mid(m_app->get_window_size().x, 256), 10, false);
	}

	m_atlas->draw(20, 20, std::to_string(m_current_frame).c_str(), m_app->get_main_font(), {255,255,255});
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

			SequencerItemTypeNames[m_current_file_idx] = event.drop.file;
			mySequence.myItems.push_back(Timeline::MySequenceItem{ m_current_file_idx, 0, 2, false });

			m_video_frames.push_back(VideoFrame{event.drop.file, m_resources->LoadTexture(event.drop.file)});

			m_current_file_idx++;

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
					m_is_playing = !m_is_playing;
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

