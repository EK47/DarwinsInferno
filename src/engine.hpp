#ifndef engine_hpp
#define engine_hpp
#pragma once

class Engine 
{
public:
    Engine( int screenWidth, int screenHeight );
    ~Engine();
    std::shared_ptr<Actor> closestCreature( std::shared_ptr<Actor> owner, int range, bool sameType ) const;
    void update();
    void render();

    int cTurns;

    std::shared_ptr<Map> map;
    std::shared_ptr<Camera> camera;
    std::vector< std::shared_ptr<Actor> > actors;
private:
};

extern Engine engine;

#endif