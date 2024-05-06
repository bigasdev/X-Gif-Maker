#include "MainScene.hpp"

#include "../ImGui/tinyfiledialogs.h"

//Used to reference the app in local functions
App* m_app_ptr = nullptr;

//Private variables
std::string m_file_path = "";
std::thread convert_thread;

std::atomic<bool> is_convertion_running(false);

//Textures
SDL_Texture* m_file_hover_tx = nullptr;
SDL_Texture* m_file_selected_tx = nullptr;
SDL_Texture* m_add_symbol_tx = nullptr;
SDL_Texture* m_bg_tx = nullptr;

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

	m_file_hover_tx = m_app->get_resources()->GetAsset("hover")->GetTexture();
	m_file_selected_tx = m_app->get_resources()->GetAsset("selected")->GetTexture();
	m_add_symbol_tx = m_app->get_resources()->GetAsset("add_symbol")->GetTexture();
	m_bg_tx = m_app->get_resources()->GetAsset("main_scene")->GetTexture();
}

void MainScene::init()
{
	m_cd->set_state("init_event", .1f, [&] {	m_logger->log("Starting the MainScene!");});
	m_app->change_background_color(vec3f(42, 47, 78));
	
	//
	//
	load_assets();
	
	//F_ASSERT(m_files.size() > 0);
}


void MainScene::update(double deltaTime)
{
	if(!m_file_path.empty()){

		std::filesystem::path path = m_file_path;
		std::string filename = path.filename().string();
		
		//yes its really hard to understand and maintain this stuff, need to rework it asap
		int x = 55*Math::clamp(1, m_files.size()+1, 8);
		if (m_files.size() == 0) x = 90;
		if( x == 110) x = 145;
		std::cout << x << "" << m_files.size() << "\n";
		FileEntity file(vec2f(x, 100), vec2f(36, 36), m_app->get_resources()->GetAsset("new_file")->GetTexture(), 0);
		file.set_file_path(filename);

		m_files.push_back(file);

		m_file_path = "";
	}

	for(auto& file : m_files){
		file.hover(file.is_close_to_pos(vec2f(Mouse::get_mouse_pos().x, Mouse::get_mouse_pos().y), 50));

		if(file.is_hovered()){
			if(m_current_mouse_key == LEFT_CLICK){
				std::cout << "Clicked" << std::endl;
				m_current_mouse_key = NO_KEY;
			}
		}
	}
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
	ImGui::SameLine();
	ImGui::Text( std::to_string(m_app_ptr->get_fps()).c_str() );
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::End();
}

//Base UI for the main scene
void base_ui(){
	auto windowSize = m_app_ptr->get_window_size();
	auto middle_of_screen = std::make_pair(windowSize.x / 2, windowSize.y);

	//Hold the powerful magic numbers to center the text!!!
	middle_of_screen.first -= 170;
	middle_of_screen.second -= 40;

	ImGui::SetNextWindowPos(ImVec2(middle_of_screen.first, middle_of_screen.second));
	ImGui::Begin("Main Scene", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	if(is_convertion_running){
		ImGui::Text("Converting...");
	} else {
		ImGui::Text("DRAG AND DROP YOUR MP4 FILES");
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

    std::string content = "video2gif.bat \"" + m_file_path + "\" " + command;
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
	m_app->get_atlas()->draw(m_bg_tx, vec2f(1200, 600), 1, 0, 0, false);
	GUI::draw([this](){this->ui();});
	//m_app->get_atlas()->draw(m_add_symbol_tx, vec2f(44, 44), 1, 544, 517, false);

	for(auto file : m_files){
		//convert file to just the entity base class
		m_app->get_atlas()->draw(&file, m_camera);

		if(file.is_hovered()){
			//std::cout << "Hovered" << std::endl;
			m_app->get_atlas()->draw(m_file_hover_tx, vec2f(14, 8), 1, file.get_pos().x, file.get_pos().y - 15, false);
		}

		m_app->get_atlas()->draw(file.get_pos().x-5, file.get_pos().y + 32, file.get_file_path().c_str(), m_app->get_main_font(), {101,115,146,255});
	}
}

void MainScene::input(SDL_Event event)
{
	//example of a input listening
	//mHero->Input(event);	

	switch (event.type) {
		case SDL_DROPFILE:
			m_file_path = event.drop.file;
			std::cout << "File dropped on window: " << m_file_path << std::endl;
			//convert_file();
		break;

		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:
					m_current_mouse_key = LEFT_CLICK;
				break;
			}

		case SDL_MOUSEBUTTONUP:
			if(event.type == SDL_MOUSEBUTTONDOWN)return;
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:			
					m_current_mouse_key = NO_KEY;
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
