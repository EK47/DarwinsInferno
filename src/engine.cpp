#include "main.hpp"

Engine::Engine( int screenWidth, int screenHeight )
{
    // USELESS Limits FPS as to protect CPU usage
    //TCODSystem::setFps( 60 );
    TCODConsole::initRoot( screenWidth, screenHeight, "Darwin's Inferno", false );
    TCODConsole::setCustomFont( "terminal16x17_gs_ro.png", TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW );

    // Creates a map
    map = std::make_shared<Map>( screenWidth * 4, screenHeight * 4 );
    // Creates a camera
    camera = std::make_shared<Camera>( 128, 112 );
    // The current turns is zero.
    cTurns = 0;
}

Engine::~Engine()
{

}

std::shared_ptr<Actor> Engine::closestCreature( std::shared_ptr<Actor> owner, int range, bool sameType ) const
{
    std::shared_ptr<Actor> closest;
    int bestDistance = 1E6;

    // Loops through actors and finds the nearest one in a range
    for( auto &actor : actors )
    {
        if( ( sameType && actor -> name != owner -> name ) || actor -> mate != nullptr || actor == owner )
        {
            continue;
        } else
        {
            float distance = hypot( actor -> x - owner -> x, actor -> y - owner -> y );
            if( distance < bestDistance && ( distance <= range || range == 0.0f ) )
            {
                bestDistance = distance;
                closest = actor;
            }
        }
    }

    return closest;
}

void Engine::update()
{
    for( auto &actor : engine.actors )
    {
        if( actor )
        {
            actor -> update( actor );
        }
    }
}

void Engine::render()
{
    TCODConsole::root -> clear();
    map -> render();

    for( auto &actor : engine.actors )
    {
        actor -> render();
    }

    camera -> render();
}

