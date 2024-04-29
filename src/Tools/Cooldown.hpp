#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <list>

#include "SDL.h"

/// <summary>
/// Class to work with routines in the main thread
/// </summary>

struct Event{
	std::string name; float timer; std::function<void()> event; bool repeat;

	float current_timer = 0; bool finished = false;

	Event() 
		:name("event"), timer(1), event(NULL), repeat(false) 
	{}
	Event(std::string _name, float _timer, std::function<void()> _event, bool _repeat)
		:name(_name), timer(_timer), event(_event), repeat(_repeat)
	{}

	void start() {
		std::cout << "created the event : " << name << "with the parameters : " << timer << repeat << std::endl;
	}

	int update(double deltaTime) {
		if (!finished) {
			current_timer += 1 * deltaTime;

			//std::cout << "updating the event : " << name << " with the timer : " << currentTimer << std::endl;

			if (current_timer >= timer) {
				std::cout << "finished the event of : " << name << std::endl;
				event();
				if (!repeat) {
					std::cout << "finishing the event : " << name << std::endl;
					finished = true;
				}
				current_timer = 0;
			}
			return 1;
		}
		return 0;
	}
};

class Cooldown {
public:
	Cooldown();

	void update(double deltaTime);

	//set
	void set_state(std::string _name, float _timer, std::function<void()> _event, bool _repeat = false);

private:
	//set variables
	std::list<Event*> m_events;

	//local variables
	float m_current_timer = 0;
};