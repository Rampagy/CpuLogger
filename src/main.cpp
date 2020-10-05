#include <chrono>
#include <unistd.h>
#include <iostream>
#include <map>
#include <thread>

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
        std::chrono::microseconds duration;

        /** Exit loop if signal interrupt is detected. */
        uint64_t i = 0;
        while ( !sigIntExit )
        {
            if (i >= settings.waitTicks)
            {
                /** Reset CPU temperatures. */
                std::map<std::string, float> temperatureInfo;
                std::map<std::string, float> fanInfo;

                /** Create thread for each data gathering function. Slowest to fastest. */
                std::thread t1(GetTemperatures, &temperatureInfo);      // 5.0 ms
                std::thread t2(GetFans, &fanInfo);                      // 3.5 ms
                std::thread t3(GetFrequency, &frequencyInfo);           // 1.5 ms

                /** Wait for each thread to finish. */
                t3.join();
                t2.join();
                t1.join();

                /** Print the results. */
                PrintResults( &frequencyInfo, &temperatureInfo, &fanInfo, duration.count() );
            }

            /** Wait designated time. */
            duration = std::chrono::duration_cast<std::chrono::microseconds>
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
