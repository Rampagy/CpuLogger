#include <chrono>
#include <iostream>

#include "arguments.hpp"
#include "frequency.hpp"
#include "unistd.h"

int main(int argc, char* argv[])
{
    /** TODO: Create graceful ctrl+c exit. */

    /** Settings that can be overwritten from the command line */
    float interval = 0.02;
    args_t settings = {
        (uint64_t)(interval*1000000),
        (uint64_t)(15/interval),
        (uint64_t)(5/interval), 
    };

    /** Parse arguments. */
    if ( !parseArguments( argc, argv, &settings ) )
    {
        /** Collect CPU frequency at specified interval. */
        float runningAverage = 0.0f;
        FREQUENCY_INFO_t cpuInfo = { 0.0f, 9999.0f, 0.0f };
        auto start = std::chrono::high_resolution_clock::now();

        /** Start sampling. */
        for (uint64_t i = 0; i < settings.waitTicks + settings.durationTicks; i++)
        {
            if (i > settings.waitTicks)
            {
                /** Only reset CPU frequency average. */
                cpuInfo.average = 0.0f;

                /** Get new cpu frequencies. */
                GetFrequency( &cpuInfo );

                /** Calculate average without saving every single value. */
                runningAverage -= runningAverage / (i - settings.waitTicks);
                runningAverage += cpuInfo.average / (i - settings.waitTicks);

                /** Make the results look pretty. */
                char buffer[50];
                std::sprintf(buffer,
                        "    Min: %0.1f MHz  Freq: %0.1f MHz  Max: %0.1f MHz",
                        cpuInfo.min,
                        cpuInfo.average,
                        cpuInfo.max);

                /** Print results. */
                std::cout << buffer << "\r" << std::flush;;
            }
            else if (i == 0)
            {
                std::cout << "    Loading...\n" << std::flush;
            }

            /** Wait designated time. */
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                    ( std::chrono::high_resolution_clock::now() - start );
            usleep( settings.evaluationInterval_us - duration.count());
            start = std::chrono::high_resolution_clock::now();
        }

        /** Print overall average. */
        char buffer[50];
        std::sprintf(buffer,
                "    Average: %0.1f MHz",
                runningAverage);
        std::cout << std::endl << buffer << std::endl;
    }
}