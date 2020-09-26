#include "frequency.hpp"

/** Go through /proc/cpuinfo and extract cpu frequencies. */
void GetFrequency( FREQUENCY_INFO_t* cpuFrequency )
{
    static uint64_t count = 1;
    std::string file;
    std::string line;
    line.reserve(2000);

    std::ifstream cpuInfo( "/proc/cpuinfo" );

    if ( cpuInfo.is_open() )
    {
        std::list<float> threadFrequencies;

        /** Iterate through each line and save to buffer. */
        while ( std::getline( cpuInfo, line ) )
        {
            if ( line.substr(0, 11) == "cpu MHz		: " )
            {
                // frequency starts on character 11 and ends on character 16
                std::string strFreq;
                for (uint8_t i = 11; i <= 16; i++)
                {
                    strFreq += line[i];
                }

                /** Convert to float. */
                float freq = std::stof( strFreq );

                /** Save the frequency. */
                threadFrequencies.push_back( freq );
                cpuFrequency->current += freq;

                if ( freq > cpuFrequency->max )
                {
                    cpuFrequency->max = freq;
                }
                if ( freq < cpuFrequency->min )
                {
                    cpuFrequency->min = freq;
                }
            }
        }

        /** Calculate outputs. */
        cpuFrequency->current /= threadFrequencies.size();

        /** Calculate average. */
        cpuFrequency->average -= cpuFrequency->average / count;
        cpuFrequency->average += cpuFrequency->current / count;
        count++;
    }
}
