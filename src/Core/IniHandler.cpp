#include "IniHandler.hpp"

#include <chrono>

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

        data.lastWriteTime = std::to_string(std::filesystem::last_write_time(path).time_since_epoch().count());


        for(auto& [key,val] : sectionPair.second){
            if(key == "fileName"){
                data.fileName = val.as<std::string>();
            }else if(key == "relative_x"){
                data.relative_x = val.as<int>();
            }else if(key == "relative_y"){
                data.relative_y = val.as<int>();
            }else if(key == "relative_name"){
                data.relative_name = val.as<std::string>();
            }else if(key == "lastWriteTime"){
                data.lastWriteTime = val.as<std::string>();
            }
        }

        ini_files.push_back(data);

    }
}

void IniHandler::update_ini_files()
{
    ini::IniFile file;
    for(auto& ini : ini_files){
        if(std::to_string(std::filesystem::last_write_time(ini.fileName).time_since_epoch().count()) != ini.lastWriteTime){
            std::cout << "INFO: Ini file was modified\n";
            file.load(ini.fileName);

            ini.name = file[ini.name]["name"].as<std::string>();
            ini.relative_x = file[ini.name]["relative_x"].as<int>();
            ini.relative_y = file[ini.name]["relative_y"].as<int>();
            ini.relative_name = file[ini.name]["relative_name"].as<std::string>();
            ini.lastWriteTime = std::to_string(std::filesystem::last_write_time(ini.fileName).time_since_epoch().count());

            update_ini_file(ini);
        }
    }
}

void IniHandler::update_ini_file(IniData data)
{
    ini::IniFile file;

    file.load(data.fileName);

    file[data.name];
    file[data.name]["name"] = data.name;
    file[data.name]["fileName"] = data.fileName;
    file[data.name]["relative_x"] = std::to_string(data.relative_x);
    file[data.name]["relative_y"] = std::to_string(data.relative_y);
    file[data.name]["relative_name"] = data.relative_name;
    file[data.name]["lastWriteTime"] = std::to_string(std::filesystem::last_write_time(data.fileName).time_since_epoch().count());

    file.save(data.fileName);
}

void IniHandler::create_ini_file(IniData data, std::string path)
{
    ini::IniFile file;

    //check if the file exists
    if(!std::filesystem::exists(path)){
        data.lastWriteTime = "0";
    }else{
        data.lastWriteTime = std::to_string(std::filesystem::last_write_time(path).time_since_epoch().count());
    }

    file.load(path);

    file[data.name];
    file[data.name]["name"] = data.name;
    file[data.name]["fileName"] = data.fileName;
    file[data.name]["relative_x"] = std::to_string(data.relative_x);
    file[data.name]["relative_y"] = std::to_string(data.relative_y);
    file[data.name]["relative_name"] = data.relative_name;
    if(!std::filesystem::exists(path)){
        file[data.name]["lastWriteTime"] = "0";
    }else{
        file[data.name]["lastWriteTime"] = std::to_string(std::filesystem::last_write_time(path).time_since_epoch().count());
    }

    file.save(path);
    ini_files.push_back(data);
}

IniData IniHandler::get_ini_data(std::string name)
{
    auto start = std::chrono::high_resolution_clock::now();

    if(ini_files.size() == 0){
        IniData data;
        data.fileName = "config.ini";
        data.name = name;

        create_ini_file(data, "config.ini");
    }

    for(auto& ini : ini_files){
        if(ini.name == name){
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;

            std::cout << "INFO: Ini file found in " << elapsed.count() << " nanoseconds\n";

            return ini;
        }else{
            create_ini_file(IniData(), "config.ini");
        }
    }
    
    return IniData();
}
