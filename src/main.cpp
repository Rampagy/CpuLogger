#include "frequency.hpp"
#include "unistd.h"


int main(int argc, char** argv)
{
    /** TODO: Create argument parsing. */

    /** TODO: Create graceful ctrl+c exit. */

    FREQUENCY_INFO_t cpuInfo;

    for (int i = 0; i < 2; i++)
    {
        cpuInfo = { 0.0f, 9999.0f, 0.0f };
        GetFrequency( &cpuInfo );

        std::cout << "Ave: " << std::to_string( cpuInfo.average ) << std::endl;
        std::cout << "Min: " << std::to_string( cpuInfo.min ) << std::endl;
        std::cout << "Max: " << std::to_string( cpuInfo.max ) << std::endl;

        usleep( 100000 );
    }
    
}