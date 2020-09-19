#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include <iostream>

typedef struct {
    uint64_t evaluationInterval_us;
    uint64_t durationTicks;
    uint64_t waitTicks;
} args_t;

uint8_t parseArguments(int argc, char* argv[], args_t* settings);
bool CheckNumeric(std::string arg);


#endif