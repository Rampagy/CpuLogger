#include "frequency.hpp"

void GetFrequency( FREQUENCY_INFO_t* cpuFrequency )
{
    std::string line;
    std::smatch match;
    std::list<float> threadFrequencies;
    std::regex expression( "cpu MHz		: (.*)" );
    std::ifstream cpuInfo( "/proc/cpuinfo" );

    if ( cpuInfo.is_open() )
    {
        /** Iterate through each line. */
        while ( std::getline( cpuInfo, line ) )
        {
            /** When the pattern is found save the frequency. */
            if ( std::regex_search( line, match, expression ) )
            {

                /** Get the frequency and convert to float. */
                float freq = std::stof( match[1] );

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
        }

        /** Close file. */
        cpuInfo.close();

        /** Calculate outputs. */
        cpuFrequency->average /= threadFrequencies.size();
    }
}