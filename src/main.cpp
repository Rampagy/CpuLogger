#include <chrono>

#include "frequency.hpp"
#include "unistd.h"

int main(int argc, char** argv)
{
    /** TODO: Create argument parsing. */

    /** TODO: Create graceful ctrl+c exit. */

    /** TODO: Add function for printing data. */

    /** settings that can be set from the command line */
    uint64_t waitTime = 5;
    uint64_t evaluationTime = 15;
    float evaluationInterval = 0.1;

    /** Convert from seconds to microseconds/ticks. */
    uint64_t evaluationInterval_us = evaluationInterval * 1000000;
    uint64_t totalTicks = (waitTime + evaluationTime) / evaluationInterval;
    uint64_t waitTicks = waitTime / evaluationInterval;

    FREQUENCY_INFO_t cpuInfo = { 0.0f, 9999.0f, 0.0f };

    auto start = std::chrono::high_resolution_clock::now();

    for (uint64_t i = 0; i < totalTicks; i++)
    {
        if (i > waitTicks)
        {
            /** Only reset CPU frequency average. */
            cpuInfo.average = 0.0f;

            /** Get new cpu frequencies. */
            GetFrequency( &cpuInfo );

            /** Make results look pretty. */
            char buffer[50];
            std::sprintf(buffer,
                    "    Min: %0.1f  Freq: %0.1f  Max: %0.1f",
                    cpuInfo.min,
                    cpuInfo.average,
                    cpuInfo.max);

            /** Print results. */
            std::cout << buffer << "\r" << std::flush;;
        }
        else if (i == 0)
        {
            std::cout << "  Loading...\r" << std::flush; 
        }

        /** Wait designated time. */
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                ( std::chrono::high_resolution_clock::now() - start );
        usleep( evaluationInterval_us - duration.count());
        start = std::chrono::high_resolution_clock::now(); 
    }
    std::cout << "\n";
}