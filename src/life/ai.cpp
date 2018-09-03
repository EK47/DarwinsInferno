#include "../main.hpp"

static const int MATING_AGE = 100;
static const int DEATH_AGE = 200;

Ai::Ai()
{

}

Ai::~Ai()
{

}

// They can make babies but if they make one with themself it crashes I think.
// At least it kind of works?

void Ai::update( std::shared_ptr<Actor> owner )
{
    // Try and mate.
    owner -> mateWith( owner );
    // Just pathfind to wherever the actor wants to go
    owner -> ai -> pathFind( owner, rand() % engine.map -> width, rand() % engine.map -> height );
}

void Ai::pathFind( std::shared_ptr<Actor> owner, int targetX, int targetY )
{
    // Pathfinds to a point.
    engine.map -> mapPath -> compute( owner -> x, owner -> y, targetX, targetY );
    if( engine.map -> mapPath -> size() > 0 )
    {
        engine.map -> mapPath -> walk( &( owner -> x ), &( owner -> y ), false );
    }
}