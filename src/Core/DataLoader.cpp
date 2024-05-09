#include "DataLoader.hpp"

const char *Data_Loader::load_file(const char *filter)
{
    char const * lFilterPatterns[1]={filter};
    auto file = tinyfd_openFileDialog("Open File", NULL, 1, lFilterPatterns, NULL, 0);

    if (!file) {
        std::cout << "No file selected" << std::endl;
        return "";
    }

	return file;
}

const char *Data_Loader::load_folder(const char *title)
{   
    auto folder = tinyfd_selectFolderDialog(title, SDL_GetBasePath());

    if (!folder) {
        std::cout << "No folder selected" << std::endl;
        return "";	
    }

    return folder;
}
