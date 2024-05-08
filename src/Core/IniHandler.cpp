#include "IniHandler.hpp"

#include "Assert.h"

IniHandler::IniHandler()
{
    std::cout << "INFO: Started IniHandler\n";
}

IniHandler::~IniHandler()
{
}

void IniHandler::load_ini_files(std::string path)
{
    ini::IniFile file;

    file.load(path);

    for(const auto& sectionPair : file){
        IniData data;
        data.fileName = path;
        data.name = sectionPair.first;

        std::unordered_map<std::string, std::string> fieldMap = {
            {"fileName", data.fileName},
            {"relative_x", std::to_string(data.relative_x)},
            {"relative_y", std::to_string(data.relative_y)},
            {"relative_name", data.relative_name},
        };

        for(auto& [key,val] : sectionPair.second){
            if(fieldMap.count(key) > 0) {
                fieldMap[key] = val.as<std::string>();
            }
        }

        data.fileName = fieldMap["fileName"];
        data.relative_x = std::stoi(fieldMap["relative_x"]);
        data.relative_y = std::stoi(fieldMap["relative_y"]);
        data.relative_name = fieldMap["relative_name"];

        F_ASSERT(data.name != "");
        ini_files.push_back(data);
    }
}

void IniHandler::update_ini_files()
{
    if(ini_files.size() == 0)return;

    ini::IniFile file;

    file.load("config.ini");

    if(file["config"]["lastWriteTime"].as<std::string>() != std::to_string(std::filesystem::last_write_time("config.ini").time_since_epoch().count())){

        for(auto& ini : ini_files){
            ini.name = file[ini.name]["name"].as<std::string>();
            ini.relative_x = file[ini.name]["relative_x"].as<int>();
            ini.relative_y = file[ini.name]["relative_y"].as<int>();
            ini.relative_name = file[ini.name]["relative_name"].as<std::string>();

            update_ini_file(ini);
        }

        file["config"]["lastWriteTime"] = std::to_string(std::filesystem::last_write_time("config.ini").time_since_epoch().count());
        file.save("config.ini");
    }
}

void IniHandler::update_ini_file(IniData data)
{
    F_ASSERT(data.name != "");
    ini::IniFile file;

    if(data.name == "")return;

    file.load(data.fileName);

    file[data.name];
    file[data.name]["name"] = data.name;
    file[data.name]["fileName"] = data.fileName;
    file[data.name]["relative_x"] = std::to_string(data.relative_x);
    file[data.name]["relative_y"] = std::to_string(data.relative_y);
    file[data.name]["relative_name"] = data.relative_name;

    file.save(data.fileName);
}

void IniHandler::create_ini_file(IniData data, std::string path)
{
    ini::IniFile file;

    std::cout << "INFO: Creating ini file " << path << data.name << "\n";

    file.load(path);

    file[data.name];
    file[data.name]["name"] = data.name;
    file[data.name]["fileName"] = data.fileName;
    file[data.name]["relative_x"] = std::to_string(data.relative_x);
    file[data.name]["relative_y"] = std::to_string(data.relative_y);
    file[data.name]["relative_name"] = data.relative_name;

    if(ini_files.size() > 0){
        file["config"]["lastWriteTime"] = std::to_string(std::filesystem::last_write_time(data.fileName).time_since_epoch().count());
    }

    file.save(path);
    ini_files.push_back(data);
}

IniData IniHandler::get_ini_data(std::string name)
{
    for(auto& ini : ini_files){
        if(ini.name == name){
            return ini;
        }
    }

    IniData data;
    data.fileName = "config.ini";
    data.name = name;

    create_ini_file(data, "config.ini");
    
    return data;
}
