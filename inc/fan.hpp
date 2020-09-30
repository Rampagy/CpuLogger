#if !defined(FAN_HPP)
#define FAN_HPP

#include <string>
#include <map>
#include <filesystem>
#include <fstream>

void GetFans( std::map<std::string, float>* fanInfo );

#endif