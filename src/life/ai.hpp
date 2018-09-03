#ifndef ai_hpp
#define ai_hpp
#pragma once

// Ai class. Controls the actions of actors.
class Ai
{
public:
    Ai();
    ~Ai();
    void update( std::shared_ptr<Actor> owner );
    void pathFind( std::shared_ptr<Actor> owner, int targetX, int targetY );
};

#endif