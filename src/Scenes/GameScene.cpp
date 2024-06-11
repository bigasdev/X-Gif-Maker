#include "GameScene.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include "../Utils/Vec.hpp"
#include "../Utils/Math.hpp"
#include "ConvertionHandler.hpp"

SDL_Texture* m_bg_texture = nullptr;
SDL_Texture* m_left_door = nullptr;
SDL_Texture* m_right_door = nullptr;

std::vector<std::string> m_files_path;

int m_current_file_idx = 0;

struct GifFrame{
	std::string m_file_path;
	std::string m_file_name;
	SDL_Texture* m_texture;
	int frame_start;
	int frame_end;
};

Timeline m_timeline;
std::vector<GifFrame> m_video_frames;

//control variables
std::string m_current_extension = "";

//preview variables
int m_current_frame = 0;
float timer = 0.0f;
float m_frame_time = 0.5f;
int m_max_frames = 10;
bool m_is_playing = false;

//door variables
float m_right_door_x = 0;
float m_left_door_x = 0;
float m_door_speed = 3.5f;
int m_right_door_pos = 340;
int m_left_door_pos = -330;


GameScene::GameScene(App *app, Logger *logger, Cooldown *cooldown, Camera *camera):Scene(app, logger, cooldown, camera)
{
	
}

void GameScene::load_assets()
{
    m_bg_texture = m_resources->GetAsset("image_gif_scene")->GetTexture();
	m_left_door = m_resources->GetAsset("left_door")->GetTexture();
	m_right_door = m_resources->GetAsset("right_door")->GetTexture();
}

void GameScene::init()
{
    m_cd->set_state("init_event", .1f, [&] {	m_logger->log("Starting the GameScene!");});
	m_app->change_background_color({25,26,50});

    //
    load_assets();

   	m_timeline.mFrameMin = 0;
   	m_timeline.mFrameMax = 10;

	//
	m_right_door_x = 0;
	m_left_door_x = 0;
}


void GameScene::update(double deltaTime)
{
	m_left_door_x = Math::lerp(m_left_door_x, m_left_door_pos, m_door_speed * deltaTime);
	m_right_door_x = Math::lerp(m_right_door_x, m_right_door_pos, m_door_speed * deltaTime);

	if(m_files_path.size() <= 0)return;

	for(int i = 0; i < m_video_frames.size(); i++){
		int* x;
		int* y;
		m_timeline.Get(i, &x, &y, nullptr, nullptr);

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

		if(m_is_playing){
			currentFrame = m_current_frame;
		}else{
			m_current_frame = currentFrame;
		}

        //Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
		ImGui::SetNextWindowPos(ImVec2(0, 210));
		ImGui::SetNextWindowSize(ImVec2(550, 170));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
		if (ImGui::Begin("Timeline", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
			ImGui::PushItemWidth(130);
			ImGui::InputInt("Frame Min", &m_timeline.mFrameMin);
			ImGui::SameLine();
			ImGui::InputInt("Frame ", &currentFrame);
			ImGui::SameLine();
			ImGui::InputInt("Frame Max", &m_timeline.mFrameMax);
			ImGui::PopItemWidth();
			// Inside the window, you can place your UI elements or other content
			// For example:
			Sequencer(&m_timeline, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
			
			// End the window
			ImGui::End();
		}
		ImGui::SetNextWindowPos(ImVec2(552, 210));
		ImGui::SetNextWindowSize(ImVec2(254, 170));
		if(ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)){
			ImGui::End();
		}
		ImGui::PopStyleColor();
	}
}

void GameScene::draw()
{
	auto condition = [&](const GifFrame& vf){
		return m_current_frame >= vf.frame_start && m_current_frame <= vf.frame_end;
	};

	auto frame = Vec::find_vec_element<GifFrame>(m_video_frames, condition);

	if(frame != nullptr){
		m_atlas->draw(frame->m_texture, {256,192}, 1, Math::mid(m_app->get_window_size().x, 256)-18, 10, false);
	}

	m_atlas->draw(m_bg_texture, {401, 107}, 2, 0, 0, false);

	m_atlas->draw(m_left_door, {401, 107}, 2, m_left_door_x, 0, false);
	m_atlas->draw(m_right_door, {401, 107}, 2, m_right_door_x, 0, false);

	//ui
	GUI::draw([this](){this->ui();});

	for(auto &f : m_video_frames){
		m_atlas->draw(20, 35, f.m_file_name.c_str(), m_app->get_main_font(), {255,255,255});
	}

	if(m_is_playing)
		m_atlas->draw(20, 20, std::to_string(m_current_frame).c_str(), m_app->get_main_font(), {255,255,255});
}

std::string get_filename(std::string path)
{
	std::filesystem::path p(path);
	std::string file_name = p.filename().string();
	file_name = file_name.substr(0, file_name.find_last_of("."));
	return file_name;
}

std::string get_extension(std::string path){
	std::filesystem::path p(path);
	std::string extension = p.filename().string();
	extension = extension.substr(extension.find_last_of(".")+1);
	return extension;
}

std::vector<std::string> filenames_storage;

void GameScene::input(SDL_Event event)
{
	//partial scenes
	for(auto& scene : m_partial_scenes){
		scene->input(event);
	}

	switch (event.type) {
		case SDL_DROPFILE:
			if(m_current_extension == ""){
				m_current_extension = get_extension(event.drop.file);
			}

			if(m_current_extension != get_extension(event.drop.file)){
				m_logger->log("Please add a " + m_current_extension + " file!");
				return;
			}

            m_files_path.push_back(event.drop.file);

			filenames_storage.push_back(get_filename(event.drop.file));

			SequencerItemTypeNames[m_current_file_idx] = filenames_storage.back().c_str();
			m_timeline.myItems.push_back(Timeline::MySequenceItem{ m_current_file_idx, 0, 2, false });

			m_video_frames.push_back(GifFrame{event.drop.file, get_filename(event.drop.file), m_resources->LoadTexture(event.drop.file)});

			m_current_file_idx++;
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
					m_app->return_scene();
				break;
				case SDL_SCANCODE_F:
					Convertion::convert(m_timeline, m_files_path, "output", "output");
					break;
				case SDL_SCANCODE_E:
					m_files_path.clear();

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

