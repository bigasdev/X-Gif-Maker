//script that will be used to read folder information
#pragma once

#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <vector>

namespace Reader{
    std::vector<std::string> get_folders(std::string path);
    std::vector<std::string> read_file(std::string path);
}