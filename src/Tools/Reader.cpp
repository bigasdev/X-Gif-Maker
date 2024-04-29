#include "Reader.hpp"

std::vector<std::string> Reader::get_folders(std::string path)
{
        std::vector<std::string> folders;
        const std::experimental::filesystem::path realPath{ path };
        for (auto const& dir_entry : std::experimental::filesystem::directory_iterator{ realPath }){
            int extension = dir_entry.path().string().find(".");
            
            if(extension != -1){
                //folders.push_back(dir_entry.path().string());
                continue;
            }
            
            folders.push_back(dir_entry.path().string());
        }
        return folders;
}

std::vector<std::string> Reader::read_file(std::string path)
{
    std::vector<std::string> files;
        const std::experimental::filesystem::path realPath{ path };
        for (auto const& dir_entry : std::experimental::filesystem::directory_iterator{ realPath }){
            int extension = dir_entry.path().string().find(".");
            
            if(extension == -1){
                //folders.push_back(dir_entry.path().string());
                continue;
            }
            
            files.push_back(dir_entry.path().string());
        }
        return files;
}