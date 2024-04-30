#include "FileEntity.hpp"

FileEntity::FileEntity()
{
}

FileEntity::FileEntity(vec2f _pos, vec2f _scale, SDL_Texture *_texture, int _uid) : Entity(_pos, _scale, _texture, _uid)
{

}

void FileEntity::set_file_path(std::string _file_path)
{
    m_file_path = _file_path;
}

std::string FileEntity::get_file_path()
{
    return m_file_path;
}

void FileEntity::hover(bool state)
{
    m_hovered = state;
}

bool FileEntity::is_hovered()
{
    return m_hovered;
}
