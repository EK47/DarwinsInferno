#include "main.hpp"

Engine engine( 128, 112 );

int main()
{
    while( !TCODConsole::isWindowClosed() )
    {
        engine.update();
        engine.render();
        TCODConsole::flush();
    }

    return 0;
}