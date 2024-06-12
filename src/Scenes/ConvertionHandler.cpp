#include "ConvertionHandler.hpp"

void Convertion::convert(std::vector<GifFrame> frames, std::string output, std::string extension)
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

		for(int i = 0; i < frames.size(); i++){
			int sum = frames[i].frame_end - frames[i].frame_start;

			outfile << "file image" << i+1 << extension << std::endl;
    		outfile << "outpoint " << sum << std::endl;
		}

		// Close the file
		outfile.close();
	}

    batchFile << "@echo off" << std::endl;
    for (size_t i = 0; i < frames.size(); ++i) {
        std::cout << frames[i].m_file_path << std::endl;
        batchFile << "copy \"" << frames[i].m_file_path << "\" ." << std::endl;
        batchFile << "ren \"" << frames[i].m_file_path.substr(frames[i].m_file_path.find_last_of("\\") + 1) << "\" \"image" << i + 1 << extension << std::endl;
    }
    //batchFile << "ffmpeg -i image%%d.jpeg -vf \"pad=180:100:(ow-iw)/2:(oh-ih)/2\" -t 5 output.gif" << std::endl;
    batchFile << "ffmpeg -f concat -i files.txt -vf palettegen=max_colors=256:reserve_transparent=1 palette.png" << std::endl;
    batchFile << "ffmpeg -f concat -safe 0 -i files.txt -i palette.png -filter_complex \"[0:v]pad=388:377:(ow-iw)/2:(oh-ih)/2,split[s0][s1];[s0]palettegen=max_colors=256:reserve_transparent=1,paletteuse=dither=bayer[b];[s1][b]paletteuse=dither=bayer\" -t 15 " << output << std::endl;
    for (size_t i = 0; i < frames.size(); ++i) {
        batchFile << "del image" << i + 1 << extension << std::endl;
        batchFile << "del palette.png" << std::endl;
    }

    batchFile.close();

    //std::remove("convert_images.bat");

    int result = system(batchFilePath.c_str());
    if (result != 0) {
        std::cerr << "Command execution failed." << std::endl;
    }
}