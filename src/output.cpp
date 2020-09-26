#include "output.hpp"

void PrintResults( FREQUENCY_INFO_t* cpuInfo, std::map<std::string, float>* temperatureInfo )
{
    /** Format for outputting data. */
    char format[50] = "%30s:  %4.1f %-5s\n";

    /** Clear console/terminal. */
    ClearConsole();

    /** Print CPU temperatures to the terminal. */
    printf( "       Temperature\n" );
    for (std::map<std::string, float>::iterator it = temperatureInfo->begin(); it != temperatureInfo->end(); ++it)
    {
        printf( "%30s:  %6.1f %-5s\n", it->first.c_str(), it->second, "°C" );
    }

    /** Print frequencies to the terminal */
    printf( "\n" );
    printf( "       Frequency\n" );
    printf( format, "Average",  cpuInfo->average,  "MHz" );
    printf( format, "Min",      cpuInfo->min,      "MHz" );
    printf( format, "Current",  cpuInfo->current,  "MHz" );
    printf( format, "Max",      cpuInfo->max,      "MHz" );
}

void ClearConsole( void )
{
    std::cout << "\x1B[2J";
    std::cout << "\x1B[1;1H";
}