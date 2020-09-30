#include <chrono>
#include <unistd.h>
#include <iostream>
#include <map>

#include "arguments.hpp"
#include "cleanup.hpp"
#include "frequency.hpp"
#include "temperature.hpp"
#include "fan.hpp"
#include "output.hpp"

int main(int argc, char* argv[])
{
    /** Setup graceful exits. */
    signal( 0x1F, signalCallbackHandler );

    /** Setup printing. */
    InitScreen();

    /** Settings that can be overwritten from the command line. */
    float interval = 0.10;
    args_t settings = {
        (uint64_t)(interval*1000000),   // interval
        (uint64_t)(0/interval),         // wait time
    };

    /** Variables that can't reset through the loop. */
    std::map<std::string, float> frequencyInfo;

    /** Parse arguments. */
    if ( !parseArguments( argc, argv, &settings ) )
    {
        /** Collect CPU frequency at specified interval. */
        auto start = std::chrono::high_resolution_clock::now();

        /** Exit loop if signal interrupt is detected. */
        uint64_t i = 0;
        while ( !sigIntExit )
        {
            if (i >= settings.waitTicks)
            {
                /** Reset CPU temperatures. */
                std::map<std::string, float> temperatureInfo;
                std::map<std::string, float> fanInfo;

                /** Get new cpu frequencies. */
                GetFrequency( &frequencyInfo );

                /** Get new CPU temperatures. */
                GetTemperatures( &temperatureInfo );

                /** Get fan information. */
                GetFans( &fanInfo );

                /** Print the results. */
                PrintResults( &frequencyInfo, &temperatureInfo, &fanInfo );
            }

            /** Wait designated time. */
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                    ( std::chrono::high_resolution_clock::now() - start );
            int64_t sleepTime = settings.evaluationInterval_us - duration.count();
            usleep( (sleepTime > 0) * sleepTime );
            start = std::chrono::high_resolution_clock::now();

            i++;
        }
    }

    /** Restore screen. */
    RestoreScreen();
}
