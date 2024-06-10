#pragma once

#include "GameScene.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

namespace Convertion{
    void convert(Timeline mySequence, std::vector<std::string> files, std::string output_folder, std::string output_name);
}