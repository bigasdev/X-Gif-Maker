#include "SDL.h"
#include "App.hpp"

//Initialization of the app and calling all the core functions
int main(int args, char *argv[]) {
	//Fps
	Uint64 currentTick = SDL_GetPerformanceCounter();
	Uint64 lastTick = 0;
	double deltaTime = 0;
	const double desiredFPS = 60.0;
	const double frameTime = 1.0 / desiredFPS;
	double accumulatedTime = 0;

	Uint32 start_time, frame_time;
	float fps;

	//App
	std::unique_ptr<App> mApp = std::make_unique<App>();
	
	mApp->init("Fortress framework v0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 760, 500, false, false);

	while (mApp->running()) {

		//Gathering the fps before everything so it can calculate the difference for the framerate;
		lastTick = currentTick;
		currentTick = SDL_GetPerformanceCounter();
		deltaTime = (double)((currentTick - lastTick) / (double)SDL_GetPerformanceFrequency());

		start_time = SDL_GetTicks();

		//Core functions
		//The core functions are all called inside the app, more functionality is added to the app class
		//Through the scenes
		accumulatedTime += deltaTime;

		while (accumulatedTime >= frameTime) {
			mApp->handle_events();
			mApp->update(frameTime);

			accumulatedTime -= frameTime;
		
		}

		mApp->render();

		//Framerate calculation
		frame_time = SDL_GetTicks()-start_time;
  		fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;

		//Sending the fps to the app so it can be displayed
		mApp->set_fps(fps);
		mApp->load();
	}

	mApp->clean();

	return 0;
}