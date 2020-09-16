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
                /** TODO: Save rather than print. */
                std::cout << match[1] << std::endl;
            }
        }

        /** Close file */
        cpuInfo.close();
    }
}