#if !defined(FREQUENCY_HPP)
#define FREQUENCY_HPP

#include <iostream>
#include <fstream>
#include <list>

typedef struct {
    float average;
    float min;
    float max;
} FREQUENCY_INFO_t;


void GetFrequency(FREQUENCY_INFO_t* cpuFrequency);


#endif