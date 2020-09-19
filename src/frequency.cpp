#include "frequency.hpp"


/** Go through /proc/cpuinfo and extract cpu frequencies. */
void GetFrequency( FREQUENCY_INFO_t* cpuFrequency )
{
    std::string file;
    std::string line;
    line.reserve(2000);
    std::list<float> threadFrequencies;
    std::ifstream cpuInfo( "/proc/cpuinfo" );

    if ( cpuInfo.is_open() )
    {
        /** Core frequency is stored every 28 lines starting at line 8. */
        /** Iterate through each line and save to buffer. */
        uint64_t count = 1;
        uint64_t freqLine = 8;
        while ( std::getline( cpuInfo, line ) )
        {
            if (count >= freqLine)
            {
                freqLine += 28;

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
                cpuFrequency->average += freq;

                if ( freq > cpuFrequency->max )
                {
                    cpuFrequency->max = freq;
                }
                if ( freq < cpuFrequency->min )
                {
                    cpuFrequency->min = freq;
                }
            }
            count++;
        }

        /** Calculate outputs. */
        cpuFrequency->average /= threadFrequencies.size();
    }
}