#include <chrono>
#include <unistd.h>
#include <iostream>
#include <map>

#include "arguments.hpp"
#include "cleanup.hpp"
#include "frequency.hpp"
#include "temperature.hpp"
#include "output.hpp"

int main(int argc, char* argv[])
{
    /** Setup graceful exits. */
    signal( SIGINT, signalCallbackHandler );

    /** Settings that can be overwritten from the command line. */
    float interval = 0.10;
    args_t settings = {
        (uint64_t)(interval*1000000),   // interval
        (uint64_t)(0/interval),         // wait time
    };

    /** Parse arguments. */
    if ( !parseArguments( argc, argv, &settings ) )
    {
        /** Collect CPU frequency at specified interval. */
        FREQUENCY_INFO_t cpuInfo = { 0.0f, 9999.0f, 0.0f , 0.0f };
        auto start = std::chrono::high_resolution_clock::now();

        /** Exit loop if signal interrupt is detected. */
        uint64_t i = 0;
        while ( !sigIntExit )
        {
            if (i >= settings.waitTicks)
            {
                /** Reset CPU frequency average and CPU temperatures. */
                cpuInfo.current = 0.0f;
                std::map<std::string, float> temperatureInfo;

                /** Get new cpu frequencies. */
                GetFrequency( &cpuInfo );

                /** Get new CPU temperatures. */
                GetTemperatures( &temperatureInfo );

                /** Print the results. */
                PrintResults( &cpuInfo, &temperatureInfo );
            }
            else if (i == 0)
            {
                printf( "    Waiting...\n" );
            }

            /** Wait designated time. */
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                    ( std::chrono::high_resolution_clock::now() - start );
            int64_t sleepTime = settings.evaluationInterval_us - duration.count();
            usleep( (sleepTime > 0) * sleepTime );
            start = std::chrono::high_resolution_clock::now();

            i++;
        }

        /** Clear console before exiting. */
        ClearConsole();
    }
}
