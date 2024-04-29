#include "MainScene.hpp"

#include "../ImGui/tinyfiledialogs.h"

//Used to reference the app in local functions
App* m_app_ptr = nullptr;

//Private variables
std::string m_file_path = "";
std::thread convert_thread;

std::atomic<bool> is_convertion_running(false);

MainScene::MainScene(App* app, Logger* logger, Cooldown* cooldown, Camera* camera):Scene(app, logger, cooldown, camera)
{
	m_app_ptr = app;
}

void MainScene::load_assets()
{
	//example of creating the hero
	//mHero = new Hero(Vector2f(250, 100), Vector2f(8*3, 10*3), mApp->GetResources()->GetTexture("hero.png", true), 0);

	//creating an idle sprite animation and loading it to the hero
	//std::vector<std::string> idleFrames = { "hero.png", "hero1.png", "hero2.png", "hero3.png", "hero4.png", "hero5.png" };
	//double timer = 0.3f;
	//mHero->AddSpriteAnimation(SpriteAnimation("idle", idleFrames, timer));
	m_GUI_assets.load_assets(m_app->get_resources());
}

void MainScene::init()
{
	m_cd->set_state("init_event", .1f, [&] {	m_logger->log("Starting the MainScene!");});
	m_app->change_background_color(vec3f(61, 61, 61));
	
	//
	//
	load_assets();
}


void MainScene::update(double deltaTime)
{
	
}


//status bar for the app
void status_bar_ui(){
	ImGui::SetNextWindowPos(ImVec2(0, m_app_ptr->get_window_size().y -20));
	ImGui::SetNextWindowSize(ImVec2(m_app_ptr->get_window_size().x, 10));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{0.25f, 0.29f, 0.43f, 1.0f});
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, -1));
	ImGui::Begin("Status Bar UI", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Filename.mb");
	ImGui::SameLine();
	ImGui::Text( " FPS" );
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::End();
}

//Base UI for the main scene
void base_ui(){
	auto windowSize = m_app_ptr->get_window_size();
	auto middle_of_screen = std::make_pair(windowSize.x / 2, windowSize.y / 2);

	//Hold the powerful magic numbers to center the text!!!
	middle_of_screen.first -= 120;
	middle_of_screen.second -= 40;

	ImGui::SetNextWindowPos(ImVec2(middle_of_screen.first, middle_of_screen.second));
	ImGui::Begin("Main Scene", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	if(is_convertion_running){
		ImGui::Text("Converting...");
	} else {
		ImGui::Text("PRESS E TO SELECT YOUR MP4 FILE");
	}
	ImGui::End();
}

//Convert command so we can attach it to another thread
void convert_command(const std::string& strCmdText) {
    system(("CMD.exe " + strCmdText).c_str());
	is_convertion_running = false;
}

//Convert function
void convert_file(){
    std::string command = "-w 800 -q 6 -o results/$2.gif"; // Default command
    // Assuming you have defined the enum Quality { high, medium, low }
    //Quality quality = Quality::high; // Adjust this based on your needs

    // Adjust the command based on quality
    /*if (quality == Quality::medium) {
        command = "-w 600 -o results/$2.gif";
    } else if (quality == Quality::low) {
        command = "-w 450 -q 4 -o results/$2.gif";
    }*/

    std::string content = "video2gif \"" + m_file_path + "\" " + command;
    std::string strCmdText = "/K " + content;

	//Launching it in another thread and detaching it
	is_convertion_running = true;
	convert_thread = std::thread(convert_command, strCmdText);
	convert_thread.detach();
}

//the main ui
void MainScene::ui(){
	//status bar
	if(debug_mode)status_bar_ui();

	//base ui
	base_ui();
}

void MainScene::draw()
{
	//ui
	GUI::draw([this](){this->ui();});
}

void MainScene::input(SDL_Event event)
{
	//example of a input listening
	//mHero->Input(event);	

	switch (event.type) {
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
					{
						
					}

				break;
				case SDL_SCANCODE_E:
					m_file_path = Data_Loader::load_file("*.mp4");

					if(m_file_path != ""){
						convert_file();
					}
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

void MainScene::clean()
{
	
}
