#include "cleanup.hpp"

volatile bool sigIntExit = false;

void signalCallbackHandler(int signum)
{
    sigIntExit = true;
}