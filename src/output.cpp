#include "output.hpp"


void PrintResults( std::map<std::string, float>*  frequencyInfo, std::map<std::string, float>* temperatureInfo, std::map<std::string, float>* fanInfo, uint64_t duration )
{
    /** Format for outputting data. */
    char buffer[100];

    /** Track how many lines have been output to the window.    *
     *  One index per column.                                   */
    uint16_t lnCount[2] = { 0 };

    /** Clear the screen. */
    clear();

    /** Print CPU temperatures to the terminal. */
    (void)SetAttrib( true, HEADER_CONFIG );
    (void)PrintTerminal( lnCount[0], 22, "Temperature" );
    (void)SetAttrib( false, HEADER_CONFIG );
    lnCount[0]++;

    for (std::map<std::string, float>::iterator it = temperatureInfo->begin(); it != temperatureInfo->end(); ++it)
    {
        /** Clear out buffer and add new data. */
        memset( &buffer[0], 0, sizeof(buffer) );
        sprintf( buffer, "%25s: %6.1f °C", it->first.c_str(), it->second );

        /** Set attributes and print. */
        (void)SetAttrib( true, COLOR_PAIR( DEFAULT_PAIR ) );
        (void)PrintTerminal( lnCount[0], 0, buffer );
        (void)SetAttrib( false, COLOR_PAIR( DEFAULT_PAIR ) );
        lnCount[0]++;
    }

    /** Skip one line between frequencies and temperatures. */
    lnCount[0]++;

    /** Print frequencies to the terminal. */
    (void)SetAttrib( true, HEADER_CONFIG );
    (void)PrintTerminal( lnCount[0], 24, "Frequency" );
    (void)SetAttrib( false, HEADER_CONFIG );
    lnCount[0]++;

    for (std::map<std::string, float>::iterator it = frequencyInfo->begin(); it != frequencyInfo->end(); ++it)
    {
        /** Clear out buffer and add new data. */
        memset( &buffer[0], 0, sizeof(buffer) );
        sprintf( buffer, "%25s: %4.1f MHz", it->first.c_str(), it->second );

        /** Set attributes and print. */
        (void)SetAttrib( true, COLOR_PAIR( DEFAULT_PAIR ) );
        (void)PrintTerminal( lnCount[0], 0, buffer );
        (void)SetAttrib( false, COLOR_PAIR( DEFAULT_PAIR ) );
        lnCount[0]++;
    }

    /** Print fan speeds to the terminal. */
    (void)SetAttrib( true, HEADER_CONFIG );
    (void)PrintTerminal( lnCount[1], 48, "Fans" );
    (void)SetAttrib( false, HEADER_CONFIG );
    lnCount[1]++;

    for (std::map<std::string, float>::iterator it = fanInfo->begin(); it != fanInfo->end(); ++it)
    {
        /** Clear out buffer and add new data. */
        memset( &buffer[0], 0, sizeof(buffer) );
        sprintf( buffer, "%5s: %5.0f RPM", it->first.c_str(), it->second );

        /** Set attributes and print. */
        (void)SetAttrib( true, COLOR_PAIR( DEFAULT_PAIR ) );
        (void)PrintTerminal( lnCount[1], 43, buffer );
        (void)SetAttrib( false, COLOR_PAIR( DEFAULT_PAIR ) );
        lnCount[1]++;
    }

    /** Skip one line between fan speeds and miscellaneous. */
    lnCount[1]++;

    /** Print miscelaneous figures. */
    (void)SetAttrib( true, HEADER_CONFIG );
    (void)PrintTerminal( lnCount[1], 45, "Miscellaneous" );
    (void)SetAttrib( false, HEADER_CONFIG );
    lnCount[1]++;

    /** Clear out buffer and add new data. */
    memset( &buffer[0], 0, sizeof(buffer) );
    sprintf( buffer, "%5s: %4.1f ms", "Loop Time", (float)duration / 1000);

    /** Print loop duration. */
    (void)SetAttrib( true, COLOR_PAIR( DEFAULT_PAIR ) );
    (void)PrintTerminal( lnCount[1], 43, buffer );
    (void)SetAttrib( false, COLOR_PAIR( DEFAULT_PAIR ) );
    lnCount[1]++;

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
    if ( (lineCount < LINES) && (column < COLS) )
    {
        std::string modOutput;

        if ( (COLS - 1) < (column + output.size()) )
        {
            modOutput = output.substr(0, COLS - column);
        }
        else
        {
            modOutput = output;
        }

        mvaddstr( lineCount, column, modOutput.c_str() );
        ret = true;
    }

    return ret;
}

bool SetAttrib( bool turnOn, uint64_t attributes )
{
    bool changed = false;
    if ( turnOn && has_colors() == TRUE )
    {
        attron( attributes );
        changed = true;
    }
    else if ( !turnOn && has_colors() == TRUE )
    {
        attroff( attributes );
        changed = true;
    }

    return changed;
}