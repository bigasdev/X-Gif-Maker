#pragma once

#include "GameScene.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

namespace Convertion{
    void convert(std::vector<GifFrame> frames, std::string output, std::string extension = ".jpeg");
}