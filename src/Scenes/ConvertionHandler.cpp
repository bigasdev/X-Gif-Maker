#include "ConvertionHandler.hpp"

void Convertion::convert(Timeline mySequence, std::vector<std::string> m_files, std::string output_folder, std::string output_name)
{
    std::string batchFilePath = "convert_images.bat";

    std::ofstream batchFile(batchFilePath);
    if (!batchFile.is_open()) {
        std::cerr << "Failed to create/open batch file." << std::endl;
    }

	//first we try to create the files txt
	{
		std::ofstream outfile("files.txt"); // Open file for writing

		// Check if the file is successfully opened
		if (!outfile.is_open()) {
			std::cerr << "Error opening file for writing!" << std::endl;
			return;
		}

		for(int i = 0; i < mySequence.myItems.size(); i++){
			int *x;
			int *y;
			int sum;

			mySequence.Get(i, &x, &y, nullptr, nullptr);
			sum = *y-*x;

			std::cout << *x << " " << *y << " " << sum << std::endl;

			outfile << "file image" << i+1 << ".jpeg" << std::endl;
    		outfile << "outpoint " << sum << std::endl;
		}

		// Close the file
		outfile.close();
	}

    batchFile << "@echo off" << std::endl;
    for (size_t i = 0; i < m_files.size(); ++i) {
        std::cout << m_files[i] << std::endl;
        batchFile << "copy \"" << m_files[i] << "\" ." << std::endl;
        batchFile << "ren \"" << m_files[i].substr(m_files[i].find_last_of("\\") + 1) << "\" \"image" << i + 1 << ".jpeg\"" << std::endl;
    }
    //batchFile << "ffmpeg -i image%%d.jpeg -vf \"pad=180:100:(ow-iw)/2:(oh-ih)/2\" -t 5 output.gif" << std::endl;
    batchFile << "ffmpeg -f concat -i files.txt -vf \"pad=1600:1200:(ow-iw)/2:(oh-ih)/2\" -t 15 " << output_name << std::endl;
    for (size_t i = 0; i < m_files.size(); ++i) {
        batchFile << "del image" << i + 1 << ".jpeg" << std::endl;
    }

    batchFile.close();

    //std::remove("convert_images.bat");

    int result = system(batchFilePath.c_str());
    if (result != 0) {
        std::cerr << "Command execution failed." << std::endl;
    }
}