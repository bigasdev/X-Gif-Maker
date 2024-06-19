#include "MainScene.hpp"

#include "../ImGui/tinyfiledialogs.h"
#include "TestPartialScene.hpp"
#include "../Utils/Gizmos.hpp"
#include "../Utils/Fini.hpp"
#include "GameScene.hpp"

//Used to reference the app in local functions
App* m_app_ptr = nullptr;

//Private variables
std::string m_file_path = "";
std::string m_folder_path = "";
std::thread convert_thread;
std::vector<FileEntity*> m_selected_files;

static const char* m_selected_width = "800";
static const char* m_selected_quality = "5";
static const char* m_selected_fps = "20";

std::atomic<bool> is_convertion_running(false);

//Textures
SDL_Texture* m_file_hover_tx = nullptr;
SDL_Texture* m_file_selected_tx = nullptr;
SDL_Texture* m_add_symbol_tx = nullptr;
SDL_Texture* m_bg_tx = nullptr;

//Data
IniData* data_press_e = nullptr;
IniData* data_press_f = nullptr;
IniData* data_convert_button = nullptr;
IniData* data_convertion_state = nullptr;
IniData* data_folder_name = nullptr;
IniData* data_file_name = nullptr;
IniData* data_settings = nullptr;
IniData* data_remove_file = nullptr;
IniData* data_select_folder = nullptr;
IniData* data_file = nullptr;
IniData* data_folder = nullptr;
IniData* change_game_scene = nullptr;

std::unique_ptr<Fini> m_fgif_settings = nullptr;


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
	m_folder_path = SDL_GetBasePath();
	F_ASSERT(m_folder_path != "");

	m_file_hover_tx = m_resources->GetAsset("hover")->GetTexture();
	m_bg_tx = m_resources->GetAsset("main_scene")->GetTexture();
}

void MainScene::init()
{
	//m_cd->set_state("init_event", .1f, [&] {	m_logger->log("Starting the MainScene!");});
	m_app->change_background_color(vec3f(42, 47, 78));
	
	//
	//
	load_assets();

	//F_ASSERT(m_files.size() > 0);

	m_ini_handler->load_ini_files("config.ini");

	m_fgif_settings = std::make_unique<Fini>("res/data/gif_settings.ini");

	//gif settings initialize
	{
		m_fgif_settings->initialize_value("gif_settings", "width", "800");
		m_fgif_settings->initialize_value("gif_settings", "quality", "5");
		m_fgif_settings->initialize_value("gif_settings", "fps", "20");

		m_fgif_settings->save();

		m_fgif_settings->grab_value("gif_settings", "width", &m_selected_width);
		m_fgif_settings->grab_value("gif_settings", "quality", &m_selected_quality);
		m_fgif_settings->grab_value("gif_settings", "fps", &m_selected_fps);
	}

	//Ini loading
	{
		data_press_e = m_ini_handler->get_ini_data("PressE");
		data_press_f = m_ini_handler->get_ini_data("PressF");
		data_convert_button = m_ini_handler->get_ini_data("ConvertOneButton");
		data_convertion_state = m_ini_handler->get_ini_data("ConvertionState");
		data_folder_name = m_ini_handler->get_ini_data("FolderName");
		data_file_name = m_ini_handler->get_ini_data("FileName");
		data_settings = m_ini_handler->get_ini_data("VideoSettings");
		data_remove_file = m_ini_handler->get_ini_data("RemoveFile");
		data_select_folder = m_ini_handler->get_ini_data("SelectFolder");
		data_file = m_ini_handler->get_ini_data("file");
		data_folder = m_ini_handler->get_ini_data("folder");
		change_game_scene = m_ini_handler->get_ini_data("ChangeGameScene");
	}

	for(auto& scene : m_partial_scenes){
		scene->init();
	}
}

//Convert command so we can attach it to another thread
void convert_command(const std::string& strCmdText) {
    system(("CMD.exe " + strCmdText).c_str());

	if(m_folder_path != ""){
		//open the folder path
		std::string command = "explorer " + m_folder_path;
		system(command.c_str());
	}

	is_convertion_running = false;
}

//Convert function
void convert_file(std::string file){
	std::filesystem::path path = file;
	std::string filename = path.filename().string();
	std::string filenameWithoutExtension = filename.substr(0, filename.find_last_of("."));


	std::string command = "-w " + std::string(m_selected_width) + " -f " + m_selected_fps + " -q " + m_selected_quality + " -o \"" + m_folder_path + "\\" + filenameWithoutExtension + ".gif\""; // Default command

	//std::string command = "-w 1024 -q 6 -o \"" + m_folder_path + "\\" + filenameWithoutExtension + ".gif\""; // Default command
	std::cout << command << std::endl;
    // Assuming you have defined the enum Quality { high, medium, low }
    //Quality quality = Quality::high; // Adjust this based on your needs

    // Adjust the command based on quality
    /*if (quality == Quality::medium) {
        command = "-w 600 -o results/$2.gif";
    } else if (quality == Quality::low) {
        command = "-w 450 -q 4 -o results/$2.gif";
    }*/

    std::string content = "video2gif.bat \"" + file + "\" " + command;
    std::string strCmdText = "/K " + content;

	//Launching it in another thread and detaching it
	is_convertion_running = true;
	convert_thread = std::thread(convert_command, strCmdText);
	convert_thread.detach();
}

void MainScene::update(double deltaTime)
{
	if(is_convertion_running) return;
	//system
#if F_ENABLE_DEBUG
	m_ini_handler->update_ini_files();
#endif

	//partial scenes
	for(auto& scene : m_partial_scenes){
		scene->update(deltaTime);
	}

	//buttons
	{
		if(Mouse::is_at_area({data_convert_button->relative_x, data_convert_button->relative_y, 156, 78})){
			std::cout << "Mouse is at the button" << std::endl;
			if(m_current_mouse_key == LEFT_CLICK){
				if(m_file_path != ""){
					convert_file(m_file_path);
				}
				m_current_mouse_key = NO_KEY;
			}
		}
		if(Mouse::is_at_area({data_remove_file->relative_x, data_remove_file->relative_y, 50, 50})){
			if(m_current_mouse_key == LEFT_CLICK){
				if(m_file_path != ""){
					m_file_path = "";
				}else{
					m_file_path = Data_Loader::load_file("*.mp4");
				}
				m_current_mouse_key = NO_KEY;
			}
		}
		if(Mouse::is_at_area({data_select_folder->relative_x, data_select_folder->relative_y, 24, 24})){
			if(m_current_mouse_key == LEFT_CLICK){
				m_folder_path = Data_Loader::load_folder("Select a folder");
				m_current_mouse_key = NO_KEY;
			}
		}
		if(Mouse::is_at_area({change_game_scene->relative_x, change_game_scene->relative_y, 24, 24})){
			if(m_current_mouse_key == LEFT_CLICK){
				m_app->change_scene(new GameScene(m_app, m_logger, m_cd, m_camera));
				m_current_mouse_key = NO_KEY;
			}
		}
	}

	m_fgif_settings->set_value("gif_settings", "width", m_selected_width);
	m_fgif_settings->set_value("gif_settings", "quality", m_selected_quality);
	m_fgif_settings->set_value("gif_settings", "fps", m_selected_fps);
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
	ImGui::SameLine();
	ImGui::Text( "Mouse Pos: " );
	ImGui::SameLine();
	ImGui::Text( std::to_string(Mouse::get_mouse_pos().x).c_str() );
	ImGui::SameLine();
	ImGui::Text( std::to_string(Mouse::get_mouse_pos().y).c_str() );
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::End();
}

//Base UI for the main scene
void base_ui(){
	auto windowSize = m_app_ptr->get_window_size();
	auto middle_of_screen = std::make_pair(windowSize.x / 2, windowSize.y);

	//Hold the powerful magic numbers to center the text!!!
	middle_of_screen.first -= 130;
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

void video_settings(){
	ImGui::SetNextWindowPos(ImVec2(	data_settings->relative_x, data_settings->relative_y));
	ImGui::SetNextWindowSize(ImVec2(200, 600));
	ImGui::Begin("Video Settings", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Video Settings");
	const char* width[] = { "200", "400", "600", "800", "1024" };
	const char* quality[] = { "1", "2", "3", "4", "5", "6" };
	const char* fps[] = { "10", "15", "20", "25", "30", "35", "40", "45" };
	ImGui::Text("Gif Width:");
	if (ImGui::BeginCombo("##select_width", m_selected_width)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(width); n++)
		{
			bool is_selected = (m_selected_width == width[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(width[n], is_selected))
				m_selected_width = width[n];
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo();
	}
	ImGui::Text("Gif Quality:");
	if (ImGui::BeginCombo("##select_quality", m_selected_quality)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(quality); n++)
		{
			bool is_selected = (m_selected_quality == quality[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(quality[n], is_selected))
				m_selected_quality = quality[n];
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo();
	}
	ImGui::Text("Gif FPS:");
	if (ImGui::BeginCombo("##select_fps", m_selected_fps)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(fps); n++)
		{
			bool is_selected = (m_selected_fps == fps[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(fps[n], is_selected))
				m_selected_fps = fps[n];
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo();
	}
	ImGui::End();

}

//the main ui
void MainScene::ui(){
	//status bar
	if(debug_mode)status_bar_ui();

	//base ui
	//base_ui();
	video_settings();
}

void MainScene::draw()
{
	//partial scenes

	//ui
	m_atlas->draw(m_bg_tx, m_app->get_window_size(), 1, 0, 0, false);
	GUI::draw([this](){this->ui();});
	//m_app->get_atlas()->draw(m_add_symbol_tx, vec2f(44, 44), 1, 544, 517, false);

	//tutorial texts
	{
		m_atlas->draw(data_press_e->relative_x, data_press_e->relative_y, "Select file", m_app->get_main_font(), {255,255,255,125});
		m_atlas->draw(data_press_f->relative_x, data_press_f->relative_y, "Choose destination", m_app->get_main_font(), {255,255,255,125});
	}

	//drawing the buttons
	Gizmos::draw_area(vec2f(data_convert_button->relative_x, data_convert_button->relative_y), 90, m_atlas, {255,0,0});
	Gizmos::draw_area(vec2f(data_select_folder->relative_x, data_select_folder->relative_y), 24, m_atlas, {255,0,0});
	Gizmos::draw_area(vec2f(data_remove_file->relative_x, data_remove_file->relative_y), 50, m_atlas, {255,0,0});
	Gizmos::draw_area(vec2f(change_game_scene->relative_x, change_game_scene->relative_y), 24, m_atlas, {255,0,0});


	if(m_folder_path != ""){
		std::string folder_name = m_folder_path.substr(m_folder_path.find_first_of("\\")+1, 38);

		m_atlas->draw(data_folder_name->relative_x, data_folder_name->relative_y, folder_name.c_str(), m_app->get_main_font(), {255,255,255,255});
	}

	if(m_file_path != ""){
		std::filesystem::path path = m_file_path;
		std::string filename = path.filename().string();
		std::string filenameWithoutExtension = filename.substr(0, filename.find_last_of("."));
		filenameWithoutExtension = filenameWithoutExtension.substr(0, 38);

		m_atlas->draw(data_file_name->relative_x, data_file_name->relative_y, filenameWithoutExtension.c_str(), m_app->get_main_font(), {255,255,255,255});
	}

	if(!is_convertion_running){
		m_atlas->draw(m_resources->GetAsset("ready_bar")->GetTexture(), vec2f(269, 16), 3, 0, 0 , false);
	}else{
		m_atlas->draw(m_resources->GetAsset("stop_vfx")->GetTexture(), vec2f(268, 20), 3, 0, 0 , false);
		m_atlas->draw(m_resources->GetAsset("stop_bar")->GetTexture(), vec2f(259, 4), 3, 12, 15 , false);
	}

		//convertion state
	m_atlas->draw(data_convertion_state->relative_x, data_convertion_state->relative_y, is_convertion_running ? "Converting..." : "READY TO CONVERT", m_app->get_main_font(), {255,255,255,255});

	//file and folder feedback
	m_atlas->draw(m_folder_path == "" ? m_resources->GetAsset("folder")->GetTexture() : m_resources->GetAsset("folder_selected")->GetTexture(), vec2f(20, 15), 2, data_folder->relative_x, data_folder->relative_y , false);
	m_atlas->draw(m_file_path == "" ? m_resources->GetAsset("file")->GetTexture() : m_resources->GetAsset("file_selected")->GetTexture(), vec2f(22, 25), 2, data_file->relative_x, data_file->relative_y , false);

	//Gizmos::draw_line(vec2f(50, 50), vec2f(200, 200), m_app->get_atlas(), {255,0,0});
	//Gizmos::draw_circle(vec2f(200, 200), 50, m_app->get_atlas(), {255,0,0});
	//Gizmos::draw_circle(m_test_entity->get_pos(), 15, m_app->get_atlas(), {0,255,0});
	for(auto& scene : m_partial_scenes){
		scene->draw();
	}
}

void MainScene::input(SDL_Event event)
{
	//example of a input listening
	//mHero->Input(event);	

	if(is_convertion_running) return;

	//partial scenes
	for(auto& scene : m_partial_scenes){
		scene->input(event);
	}

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

				break;
				case SDL_SCANCODE_F:
					m_folder_path = Data_Loader::load_folder("Select a folder");
					break;
				case SDL_SCANCODE_E:
					m_file_path = Data_Loader::load_file("*.mp4");

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

void MainScene::clean()
{
	//partial scenes
	for(auto& scene : m_partial_scenes){
		scene->clean();
	}

	m_fgif_settings->save();

	m_partial_scenes.clear();
}
