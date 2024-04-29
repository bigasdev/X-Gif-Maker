#ifndef FILEENTITY_HPP
#define FILEENTITY_HPP

#include "Entity.hpp"

class FileEntity : public Entity {
public:
    FileEntity();
    FileEntity(vec2f _pos, vec2f _scale, SDL_Texture *_texture, int _uid);

    void set_file_path(std::string _file_path);
    std::string get_file_path();

private:
    std::string m_file_path;
};

#endif