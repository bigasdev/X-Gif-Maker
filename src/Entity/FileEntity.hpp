#ifndef FILEENTITY_HPP
#define FILEENTITY_HPP

#include "Entity.hpp"

class FileEntity : public Entity {
public:
    FileEntity();
    FileEntity(vec2f _pos, vec2f _scale, SDL_Texture *_texture, int _uid);

    void set_file_path(std::string _file_path, std::string path);
    std::string get_file_path();
    std::string get_complete_file_path();
    void hover(bool state);

    bool is_hovered();

private:
    std::string m_file_path;
    std::string m_complete_file_path;

    bool m_hovered = false;
};

#endif