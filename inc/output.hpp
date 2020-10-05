#if !defined(OUTPUT_HPP)
#define OUTPUT_HPP

#include <iostream>
#include <string.h>
#include <curses.h>

#include "frequency.hpp"
#include "temperature.hpp"

/** Color pair indices. */
#define HEADER_PAIR         (1)
#define DEFAULT_PAIR        (2)
#define BACKGROUND_COLOR    (255)

/** Define header config. */
#define HEADER_CONFIG       ( COLOR_PAIR( HEADER_PAIR ) | A_BOLD )


void PrintResults( std::map<std::string, float>*  frequencyInfo, std::map<std::string, float>* temperatureInfo, std::map<std::string, float>* fanInfo, uint64_t duration );

void InitScreen( void );

void RestoreScreen( void );

void PrintFrequency( uint16_t *lnCount, char* buffer, std::string label, float val, std::string units );

bool PrintTerminal( uint16_t lineCount, uint16_t column, std::string output );

bool SetAttrib( bool turnOn, uint64_t attributes );

#endif