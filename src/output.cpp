#include "output.hpp"

uint64_t lineCount = 0;

void PrintResults( FREQUENCY_INFO_t* cpuInfo, std::map<std::string, float>* temperatureInfo )
{
    /** Format for outputting data. */
    char buffer[100];

    /** Track how many lines have been output to the window. */
    uint16_t lnCount = 0;

    /** Clear the screen. */
    clear();

    /** Print CPU temperatures to the terminal. */
    attron( HEADER_CONFIG );
    mvaddstr(lnCount, 22, "Temperature");
    attroff( HEADER_CONFIG );
    lnCount++;

    for (std::map<std::string, float>::iterator it = temperatureInfo->begin(); it != temperatureInfo->end(); ++it)
    {
        if ( has_colors() == TRUE )
        {
            attron( COLOR_PAIR( DEFAULT_PAIR ) );
        }

        memset( &buffer[0], 0, sizeof(buffer) );
        sprintf( buffer, "%25s: ", it->first.c_str() );
        mvaddstr( lnCount, 0, buffer );

        memset( &buffer[0], 0, sizeof(buffer) );
        sprintf( buffer, "%6.1f ", it->second );
        addstr( buffer );

        memset( &buffer[0], 0, sizeof(buffer) );
        sprintf( buffer, "%-5s", "°C" );
        addstr( buffer );

        if ( has_colors() == TRUE )
        {
            attroff( COLOR_PAIR( DEFAULT_PAIR ) );
        }

        lnCount++;
    }

    /** Skip one line between frequencies and temperatures. */
    lnCount++;

    /** Print frequencies to the terminal. */
    attron( HEADER_CONFIG );
    mvaddstr( lnCount, 24, "Frequency" );
    attroff( HEADER_CONFIG );
    lnCount++;

    PrintFrequency( &lnCount, &buffer[0], "Min", cpuInfo->min, "MHz" );
    PrintFrequency( &lnCount, &buffer[0], "Current", cpuInfo->current, "MHz" );
    PrintFrequency( &lnCount, &buffer[0], "Max", cpuInfo->max, "MHz" );

    refresh();
}

void InitScreen( void )
{
    /** Use curses to init the screen. */
    initscr();

    /** Use curses to disable buffering and configure curses. */
    cbreak();

    /** Tell curses not to echo every command back to the screen. */
    noecho();

    /** Tell curses to turn the cursor off. */
    /** Off = 0, normal = 1, very visible = 2 */
    curs_set( 0 );

    /** Clear the screen using curses. */
    clear();

    /** Init some color pairs if terminal supports colors. */
    if ( has_colors() == TRUE )
    {
        start_color();

        init_pair(HEADER_PAIR, 15, 234);
        init_pair(DEFAULT_PAIR, 254, 234);

        bkgdset( COLOR_PAIR( DEFAULT_PAIR ) );
    }
}

void RestoreScreen( void )
{
    /** Use curses to restore the screen. */
    endwin();
}

void PrintFrequency( uint16_t *lnCount, char* buffer, std::string label, float val, std::string units )
{
    if ( ( has_colors() == TRUE ) )
    {
        attron( COLOR_PAIR( DEFAULT_PAIR ) );
    }

    memset( &buffer[0], 0, sizeof(buffer) );
    sprintf( buffer, "%25s: ", label.c_str() );
    mvaddstr( *lnCount, 0, buffer );

    memset( &buffer[0], 0, sizeof(buffer) );
    sprintf( buffer, "%4.1f ", val );
    addstr( buffer );

    memset( &buffer[0], 0, sizeof(buffer) );
    sprintf( buffer, "%-5s", units.c_str() );
    addstr( buffer );

    if ( has_colors() == TRUE )
    {
        attroff( COLOR_PAIR( DEFAULT_PAIR ) );
    }

    (*lnCount)++;
}