#include <chrono>
#include <iostream>
#include <unistd.h>
#include <map>

#include "arguments.hpp"
#include "cleanup.hpp"
#include "frequency.hpp"
#include "temperature.hpp"


/** TODO:   Add reading temperatures from these locations
 *          /sys/class/hwmon/hwmon* /temp*_input 
 *          /sys/class/hwmon/hwmon* /temp*_label 
 * 
 *  NOTE:   Will need to interate through folders 
 *          looking for things that meet the above pattern.
**/

int main(int argc, char* argv[])
{
    /** Setup graceful exits. */
    signal( SIGINT, signalCallbackHandler );

    /** Settings that can be overwritten from the command line */
    float interval = 0.02;
    args_t settings = {
        (uint64_t)(interval*1000000),   // interval
        (uint64_t)(60/interval),        // duration
        (uint64_t)(1/interval),         // wait time
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
            if (i >= settings.waitTicks)
            {
                /** Reset CPU frequency average and CPU temperatures. */
                cpuInfo.average = 0.0f;
                std::map<std::string, float> temperatureInfo;

                /** Get new cpu frequencies. */
                GetFrequency( &cpuInfo );

                /** Calculate average without saving every single value. */
                runningAverage -= runningAverage / ((i + 1) - settings.waitTicks);
                runningAverage += cpuInfo.average / ((i + 1) - settings.waitTicks);

                /** Get new CPU temperatures. */
                GetTemperatures( &temperatureInfo );

                /** Print out the CPU temperatures to see if everything works. */
                for (std::map<std::string, float>::iterator it = temperatureInfo.begin(); it != temperatureInfo.end(); ++it)
                    std::cout << it->first << " => " << it->second << std::endl;

                /** Make the results look pretty. */
                char buffer[300];
                std::sprintf(buffer,
                        "    Min: %0.1f MHz  Freq: %0.1f MHz  Max: %0.1f MHz                      ",
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
            int64_t sleepTime = settings.evaluationInterval_us - duration.count();
            usleep( (sleepTime > 0) * sleepTime );
            start = std::chrono::high_resolution_clock::now();

            /* Exit loop if signal interrupt is detected. */
            if (sigIntExit)
            {
                break;
            }
        }

        /** Print overall average. */
        char buffer[50];
        std::sprintf(buffer,
                "    Average: %0.1f MHz",
                runningAverage);
        std::cout << std::endl << buffer << std::endl;
    }
}
