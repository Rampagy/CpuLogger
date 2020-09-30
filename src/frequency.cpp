#include "frequency.hpp"

/** Go through /proc/cpuinfo and extract cpu frequencies. */
void GetFrequency( std::map<std::string, float>* frequencyInfo )
{
    std::string file;
    std::string line;
    line.reserve(2000);

    std::ifstream cpuInfo( "/proc/cpuinfo" );

    if ( cpuInfo.is_open() )
    {
        uint16_t threadCount = 0;
        float cumulativeFreq = 0;

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
                cumulativeFreq += freq;

                std::map<std::string, float>::iterator it = frequencyInfo->find("Max");
                if ( it == frequencyInfo->end() ) 
                {
                    frequencyInfo->insert( std::pair<std::string, float>("Max", freq) );
                }
                else if ( freq > it->second )
                {
                    it->second = freq;
                }

                it = frequencyInfo->find("Min");
                if ( it == frequencyInfo->end() ) 
                {
                    frequencyInfo->insert( std::pair<std::string, float>("Min", freq) );
                }
                else if ( freq < it->second )
                {
                    it->second = freq;
                }

                threadCount++;
            }
        }

        cumulativeFreq = cumulativeFreq / threadCount;
        std::map<std::string, float>::iterator it = frequencyInfo->find("Current");
        if ( it == frequencyInfo->end() )
        {
            frequencyInfo->insert( std::pair<std::string, float>("Current", cumulativeFreq) );
        }
        else
        {
            it->second = cumulativeFreq;
        }
    }
}
