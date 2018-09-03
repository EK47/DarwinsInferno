#ifndef actor_hpp
#define actor_hpp
#pragma once

// Any extension, including the body.
class Part
{
public:
    Part();
    ~Part();

    std::string name;
    int length, weight;

    enum Function
    {
        MOUTH,
        EYE,
        MOVE,
        EAR,
        CLAW,
    };
    std::vector< std::shared_ptr<Part> > parts;

    std::vector<Part::Function> function;
};

class Actor
{
public:
    Actor( char symbol, std::string name, TCODColor col );
    ~Actor();

    void update( std::shared_ptr<Actor> owner );
    void render();
    void die( std::shared_ptr<Actor> owner );
    void mateWith( std::shared_ptr<Actor> owner );
    void birth();
    bool readFile( std::string filename, std::shared_ptr<Actor> actor );

    void to_json( nlohmann::json& j );
    void from_json( nlohmann::json& j );

    int x, y, chunk, age, matingage, deathage;
    // Where the actor wants to go.
    int tgtX, tgtY;
    // The actor's most recent mate.
    std::shared_ptr<Actor> mate;
    // Has the actor ever mated?
    bool mated;

    std::string unique_id;

    char symbol;
    std::string name;
    TCODColor col;

    std::shared_ptr<ModelCell> modelCell;
    std::shared_ptr<Part> body;

    std::shared_ptr<Ai> ai;
};

#endif