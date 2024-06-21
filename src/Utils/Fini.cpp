#include "fini.hpp"

Fini::Fini(std::string file)
{
  filename = file;

  load();
}

Fini::~Fini()
{
  save();
}

bool Fini::update()
{
  if (m_last_edited != std::to_string(std::filesystem::last_write_time(filename).time_since_epoch().count()))
  {
    m_last_edited = std::to_string(std::filesystem::last_write_time(filename).time_since_epoch().count());
    return true;
  }
  return false;
}

void Fini::save()
{
  F_Debug::log("Saving ........ " + filename);
  ini.save(filename);
}

void Fini::load()
{
  F_Debug::log("Loading ........ " + filename);
  ini.load(filename);
}
