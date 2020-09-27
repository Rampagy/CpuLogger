#if !defined(OUTPUT_HPP)
#define OUTPUT_HPP

#include <iostream>
#include <string.h>
#include <curses.h>

#include "frequency.hpp"
#include "temperature.hpp"

/** Color pair indices. */
#define HOT_COLOR       (1)
#define COLD_COLOR      (2)
#define FAST_COLOR      (3)
#define SLOW_COLOR      (4)
#define HEADER_COLOR    (5)
#define DEFAULT_COLOR   (6)

/** Set the header config. */
#define HEADER_CONFIG ( A_BOLD | COLOR_PAIR( HEADER_COLOR ) )


void PrintResults( FREQUENCY_INFO_t* cpuInfo, std::map<std::string, float>* temperatureInfo );

void InitScreen( void );

void RestoreScreen( void );

void PrintFrequency( uint16_t *lnCount, char* buffer, std::string label, float val, std::string units );

#endif