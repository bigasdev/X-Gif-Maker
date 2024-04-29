#include "DataLoader.hpp"

const char *Data_Loader::load_file(const char *filter)
{
    char const * lFilterPatterns[1]={filter};
    auto file = tinyfd_openFileDialog("Open File", NULL, 1, lFilterPatterns, NULL, 0);

    if (!file) {
        std::cout << "No file selected" << std::endl;
    }

	return file;
}