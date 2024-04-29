#include "IntroScene.hpp"
#include "MainScene.hpp"

Entity* m_logo = nullptr;
float m_y_pos = -200;
vec2f m_window_size = vec2f(0, 0);

IntroScene::IntroScene(App* app, Logger* logger, Cooldown* cooldown, Camera* camera):Scene(app, logger, cooldown, camera)
{
}

//function that will load all the logo animation assets needed for the scene
SDL_Texture* logo_frames[13] = { nullptr };

void IntroScene::load_assets()
{
	logo_frames[1] = m_app->get_resources()->GetAsset("logo1")->GetTexture();
	logo_frames[0] = m_app->get_resources()->GetAsset("logo1")->GetTexture();
	logo_frames[2] = m_app->get_resources()->GetAsset("logo2")->GetTexture();
	logo_frames[3] = m_app->get_resources()->GetAsset("logo3")->GetTexture();
	logo_frames[4] = m_app->get_resources()->GetAsset("logo4")->GetTexture();
	logo_frames[5] = m_app->get_resources()->GetAsset("logo5")->GetTexture();
	logo_frames[6] = m_app->get_resources()->GetAsset("logo6")->GetTexture();
	logo_frames[7] = m_app->get_resources()->GetAsset("logo7")->GetTexture();
	logo_frames[8] = m_app->get_resources()->GetAsset("logo8")->GetTexture();
	logo_frames[9] = m_app->get_resources()->GetAsset("logo9")->GetTexture();
	logo_frames[10] = m_app->get_resources()->GetAsset("logo10")->GetTexture();
	logo_frames[11] = m_app->get_resources()->GetAsset("logo11")->GetTexture();
	logo_frames[12] = m_app->get_resources()->GetAsset("logo12")->GetTexture();
}

void IntroScene::init()
{
	m_cd->set_state("init_event", .4f, [&] {	m_logger->log("Starting the IntroScene!");});
	m_app->change_background_color(vec3f(0, 0, 0));

	m_y_pos = -200;
	std::cout << m_y_pos << std::endl;

	m_window_size = m_app->get_window_size();

	m_logo = new Entity(vec2f((-156 + m_window_size.x) / 2, (-116/2 + m_window_size.y)/2), vec2f(156, 116), m_app->get_resources()->GetAsset("logo1")->GetTexture(), 0);
	m_logo->change_visibility(false);
	load_assets();

	std::cout << "Why" << std::endl;

	m_logo->set_texture(logo_frames[1]);

	m_cd->set_state("start_event", 1.5f, [&] {
		m_logo->change_visibility(true);
	});
}

//variables for the logo animation
int frame = 1;
float mTime = 0;

void IntroScene::update(double deltaTime)
{
	if (!m_logo->is_visible())return;

	//loop between all the frames from the animation using the time variable
	{
		if (frame < 18) {
			mTime += deltaTime;
			if (mTime >= .08f) {
				mTime = 0;
				frame++;
				if(frame < 13)
					m_logo->set_texture(logo_frames[frame]);
			}
		}else{
			m_app->change_to_next_scene();
		}
	}

	if (Mouse::is_at_area(Area(m_logo->get_pos().x, m_logo->get_pos().y, m_logo->get_current_frame().w, m_logo->get_current_frame().h))) {
		m_logger->log("Mouse on logo!");
	}
}

void Gui() {
	
}

void IntroScene::draw()
{
	m_app->get_atlas()->draw(m_logo, m_camera);

	GUI::draw(Gui);
}

void IntroScene::input(SDL_Event event)
{
}

void IntroScene::clean()
{
	m_y_pos = -200;
	delete m_logo;
}
