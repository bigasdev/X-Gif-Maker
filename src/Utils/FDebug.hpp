#pragma once

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace F_Debug {
extern std::vector<std::string> logs;

void log(std::string message);

void log_group(std::string type, std::string value);
void error(std::string message);
void warning(std::string message);

void write_to_file(std::string filename);
void signal_handler(int signal);
void terminate_handler();
void setup_crash_handlers();
} // namespace F_Debug

#endif
