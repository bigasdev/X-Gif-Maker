#include "MainScene.hpp"
#include "GameScene.hpp"

#include "../ImGui/tinyfiledialogs.h"

//Used to reference the app in local functions
App* m_app_ptr = nullptr;


//Game Scene
GameScene* m_game_scene = nullptr;


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

	m_game_scene = new GameScene(m_app, m_logger, m_cd, m_camera);
	
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
	middle_of_screen.first -= 100;
	middle_of_screen.second -= 40;

	ImGui::SetNextWindowPos(ImVec2(middle_of_screen.first, middle_of_screen.second));
	ImGui::Begin("Main Scene", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("PRESS E TO START THE GAME");
	ImGui::End();
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
					m_app->change_scene(m_game_scene);
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
