#include "output.hpp"


void PrintResults( std::map<std::string, float>*  frequencyInfo, std::map<std::string, float>* temperatureInfo )
{
    /** Format for outputting data. */
    char buffer[100];

    /** Track how many lines have been output to the window. */
    uint16_t lnCount = 0;

    /** Clear the screen. */
    clear();

    /** Print CPU temperatures to the terminal. */
    attron( HEADER_CONFIG );
    (void)PrintTerminal( lnCount, 22, "Temperature" );
    attroff( HEADER_CONFIG );
    lnCount++;

    for (std::map<std::string, float>::iterator it = temperatureInfo->begin(); it != temperatureInfo->end(); ++it)
    {
        if ( has_colors() == TRUE )
        {
            attron( COLOR_PAIR( DEFAULT_PAIR ) );
        }

        memset( &buffer[0], 0, sizeof(buffer) );
        sprintf( buffer, "%25s: %6.1f °C", it->first.c_str(), it->second );
        (void)PrintTerminal( lnCount, 0, buffer );

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
    (void)PrintTerminal( lnCount, 24, "Frequency" );
    attroff( HEADER_CONFIG );
    lnCount++;

    for (std::map<std::string, float>::iterator it = frequencyInfo->begin(); it != frequencyInfo->end(); ++it)
    {
        if ( ( has_colors() == TRUE ) )
        {
            attron( COLOR_PAIR( DEFAULT_PAIR ) );
        }

        memset( &buffer[0], 0, sizeof(buffer) );
        sprintf( buffer, "%25s: %4.1f MHz", it->first.c_str(), it->second );
        (void)PrintTerminal( lnCount, 0, buffer );

        if ( has_colors() == TRUE )
        {
            attroff( COLOR_PAIR( DEFAULT_PAIR ) );
        }

        lnCount++;
    }

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

bool PrintTerminal( uint16_t lineCount, uint16_t column, std::string output )
{
    bool ret = false;
    if (lineCount < (LINES - 1) )
    {
        if ( column < (COLS - 1) )
        {
            std::string modOutput;

            if ( ( COLS - 1 ) < ( column + output.size() ) )
            {
                modOutput = output.substr(0, COLS);
            }
            else
            {
                modOutput = output;
            }

            mvaddstr( lineCount, column, modOutput.c_str() );
            ret = true;
        }
    }

    return ret;
}