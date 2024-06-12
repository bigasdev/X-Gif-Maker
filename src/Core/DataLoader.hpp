#ifndef __DATA_LOADER__
#define __DATA_LOADER__

#include "../ImGui/tinyfiledialogs.h"
#include "SDL.h"

#include <iostream>
#include <string>
#include <cstddef>

namespace Data_Loader{
    const char* load_file(const char* filter);
    const char* load_folder(const char* title);
    const char* save_file(const char* filter);
}

#endif