#pragma once

#ifndef INIHANDLER_HPP
#define INIHANDLER_HPP

#include "../Utils/inicpp.h"
#include "../Utils/Common.hpp"

#include <filesystem>
#include <string>
#include <vector>

//Base class to handle all the ini files the project needs
//It will load an have an update class to update the ini struct if the file was modified

struct IniData{
    std::string fileName;
    std::string name;
    int relative_x = 0;
    int relative_y = 0;
    std::string relative_name = "null";

    std::string lastWriteTime;

    bool was_modified = false;
};

class IniHandler{
public:
    IniHandler();
    ~IniHandler();

    void load_ini_files(std::string path);
    void update_ini_files();

    void update_ini_file(IniData data);

    void create_ini_file(IniData data, std::string path);
    IniData get_ini_data(std::string name);

private:
    std::vector<IniData> ini_files;
};

#endif