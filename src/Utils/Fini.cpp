#include "fini.hpp"

Fini::Fini(std::string file){
    filename = file;

    ini.load(filename);
}

Fini::~Fini(){
    save();
}

bool Fini::update()
{
    if(m_last_edited != std::to_string(std::filesystem::last_write_time(filename).time_since_epoch().count())){
        m_last_edited = std::to_string(std::filesystem::last_write_time(filename).time_since_epoch().count());
        return true;
    }
    return false;
}

void Fini::save()
{
    ini.save(filename);
}

void Fini::load()
{
    ini.load(filename);
}
