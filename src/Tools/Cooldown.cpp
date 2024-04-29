#include "Cooldown.hpp"

Cooldown::Cooldown()
{
	
}

void Cooldown::update(double deltaTime)
{
	if (m_events.size() == 0)return;

	//no memory leak approach
	for (auto it = m_events.begin(); it != m_events.end();) {
		Event* ev = *it;

		ev->update(deltaTime);

		if (ev->finished) {
			if (ev) {
				std::cout << "Removing event of the list : " << ev->name << std::endl;
				delete ev;
				ev = nullptr;
			}
			it = m_events.erase(it);
		}
		else {
			++it;
		}
	}
}

void Cooldown::set_state(std::string _name, float _timer, std::function<void()> _event, bool _repeat)
{
	for (auto const& e : m_events) {
		if (e->name == _name) {
			//std::cout << "trying to add an defined event already! skipping this iteration.." << e->name << std::endl;
			return;
		}
	}
	m_events.push_back(new Event(_name, _timer, _event, _repeat));
}
