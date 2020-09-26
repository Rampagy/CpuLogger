#if !defined(OUTPUT_HPP)
#define OUTPUT_HPP

#include <iostream>

#include "frequency.hpp"
#include "temperature.hpp"

void PrintResults( FREQUENCY_INFO_t* cpuInfo, std::map<std::string, float>* temperatureInfo );

void ClearConsole( void );

#endif