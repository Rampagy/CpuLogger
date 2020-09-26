#if !defined(FREQUENCY_HPP)
#define FREQUENCY_HPP

#include <fstream>
#include <list>

typedef struct {
    float current;
    float min;
    float max;
    float average;
} FREQUENCY_INFO_t;


void GetFrequency(FREQUENCY_INFO_t* cpuFrequency);


#endif