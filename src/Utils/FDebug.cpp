#include "FDebug.hpp"
#include <csignal>
#include <exception>

namespace F_Debug
{
  std::vector<std::string> logs;
}

void F_Debug::log(std::string message)
{
  logs.push_back(message);
#if F_ENABLE_DEBUG
  std::cout << "LOG : " << message << "\n";
#endif
}

void F_Debug::log_group(std::string type, std::string value)
{
  logs.push_back(type + " : " + value);
#if F_ENABLE_DEBUG
  std::cout << std::left << std::setfill('.') << std::setw(20) << type << std::right << std::setfill('.') << std::setw(20) << value << "\n";
#endif
}

void F_Debug::error(std::string message)
{
  logs.push_back("ERROR : " + message);
#if F_ENABLE_DEBUG
  std::cerr << "ERROR : " << message << "\n";
#endif
}

void F_Debug::write_to_file(std::string filename)
{
  std::ofstream file;
  file.open(filename);
  for (std::string log : logs)
  {
    file << log << "\n";
  }
  file.close();
}

void F_Debug::signal_handler(int signal)
{
  error("Signal " + std::to_string(signal) + " received.");
  write_to_file("log.txt");
  std::_Exit(signal);
}

void F_Debug::terminate_handler()
{
  error("Unhandled exception caught.");
  write_to_file("log.txt");
}

void F_Debug::setup_crash_handlers()
{
  std::signal(SIGSEGV, signal_handler);
  std::signal(SIGABRT, signal_handler);
  std::signal(SIGFPE, signal_handler);
  std::signal(SIGILL, signal_handler);
  std::signal(SIGINT, signal_handler);
  std::signal(SIGTERM, signal_handler);

  std::set_terminate(terminate_handler);
}
