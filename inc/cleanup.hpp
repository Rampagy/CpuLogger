#if !defined(CLEANUP_HPP)
#define CLEANUP_HPP

#include <signal.h>

extern volatile bool sigIntExit;

void signalCallbackHandler(int signum);


#endif