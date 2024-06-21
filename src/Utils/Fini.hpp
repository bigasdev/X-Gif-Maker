#ifndef FINI_HPP
#define FINI_HPP

#include "inicpp.h"
#include "FDebug.hpp"
#include <iostream>
#include <string>

#include <fstream>
#include <iostream>
#include <filesystem>

/// FINI SYSTEM
// Use it to read and save ini files with ease
// it needs to have a setup to initialize the values with default values that you need
// after the initial setup it needs to be saved to keep the values updated;
///

class Fini
{
public:
  Fini(std::string file);
  ~Fini();

  bool update();
  void save();
  void load();

  template <typename T>
  void initialize_value(std::string section, std::string key, T value);

  template <typename T>
  void grab_value(std::string section, std::string key, T *var);

  template <typename T>
  T get_value(std::string section, std::string key);

  template <typename T>
  void set_value(std::string section, std::string key, T value);

  std::string filename = "fini.ini";

private:
  ini::IniFile ini;

  std::string m_last_edited;
};

template <typename T>
inline void Fini::initialize_value(std::string section, std::string key, T value)
{
  if (ini[section][key].as<std::string>().empty())
  {
    ini[section][key] = value;
  }
}

template <typename T>
inline void Fini::grab_value(std::string section, std::string key, T *var)
{
  try
  {
    *var = ini[section][key].as<T>();
  }
  catch (std::exception e)
  {
    F_Debug::error("Grab value failed: " + std::string(e.what()) + " for " + section + " " + key + " with value " + ini[section][key].as<std::string>());

    *var = ini[section][key].as<T>();
  }
}

template <typename T>
inline T Fini::get_value(std::string section, std::string key)
{
  try
  {
    return ini[section][key].as<T>();
  }
  catch (std::exception e)
  {
    F_Debug::error("Get value failed: " + std::string(e.what()) + " for " + section + " " + key + " with value " + ini[section][key].as<std::string>());
  }
}

template <typename T>
inline void Fini::set_value(std::string section, std::string key, T value)
{
  ini[section][key] = value;
}

#endif