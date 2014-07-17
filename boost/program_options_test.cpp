/*

Compiling:
g++ -std=c++0x program_options_test.cpp -o program_options_test -lboost_program_options

*/

#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <string>

namespace po = boost::program_options;

void write_settings(const char* value)
{
  std::ofstream settings_file("settings.ini");
  settings_file << "name = " << value;
}

void read_settings(po::options_description& desc,
                   po::variables_map& vm)
{
  std::ifstream settings_file("settings.ini");

  // Clear the map.
  vm = po::variables_map();

  po::store(po::parse_config_file(settings_file , desc), vm);
  po::notify(vm);    
}

int main()
{
  std::string name;

  // Setup options.
  po::options_description desc("Options");
  desc.add_options()
    ("name", po::value<std::string>(&name), "name");
  po::variables_map vm;

  // Write, read, and print settings.
  write_settings("test");
  read_settings(desc, vm);
  std::cout << "name = " << name << std::endl;

  // Write, read, and print newer settings.
  write_settings("another test");
  read_settings(desc, vm);
  std::cout << "name = " << name << std::endl;

  return 0;
}