#include "../main.hpp"

Part::Part()
{

}

Part::~Part()
{

}

Actor::Actor( char symbol, std::string name, TCODColor col ) : symbol( symbol ), name( name ), col( col ), age( 0 ), mated( false )
{
    //modelCell = std::make_shared<ModelCell>();
    body = std::make_shared<Part>();
    ai = std::make_shared<Ai>();

    unique_id = "Wolf";
}

Actor::~Actor()
{

}

void Actor::birth()
{
    std::shared_ptr<Actor> temp = std::make_shared<Actor>( '0', "0", TCODColor::black );

    temp -> readFile( name + ".json", temp );

    temp -> x = x;
    temp -> y = y;

    engine.actors.push_back( temp );
}

void Actor::mateWith( std::shared_ptr<Actor> owner )
{
    // If the creature is not dead and old enough to mate
    if( owner -> age > owner -> matingage && owner -> age < owner -> deathage && !owner -> mated )
    {
        // Find the nearest creature of the same kind
        std::shared_ptr<Actor> target = engine.closestCreature( owner, 0, true );

        // If the target exists, the owner has a mate, and the target is old enough to mate.
        if( target && owner -> mate == nullptr && target -> age > target -> matingage )
        {
            owner -> mate = target;
        }

        // If there are no possible mates, target = nullptr. If creature has mated, skip.
        if( owner -> mate != nullptr && owner -> mate -> age > owner -> matingage && ( owner -> x == owner -> mate -> x && owner -> y == owner -> mate -> y ) )
        {
            std::cout << owner -> unique_id << " birthed children with " << owner -> mate -> unique_id << std::endl;
            // Birth pups
            for( int i = 0; i < 1 + rand() % 1; ++i )
            {
                owner -> birth();
            }

            // The animals can only mate once.
            owner -> mated = true;
            owner -> mate -> mated = true;

        // If a mate exists, but hasn't been reached, then pathfind towards it.
        } else if( owner -> mate )
        {
            owner -> tgtX = owner -> mate -> x;
            owner -> tgtY = owner -> mate -> y;
            owner -> ai -> pathFind( owner, owner -> tgtX, owner -> tgtY );
        }
    // If the animal is too old, it must DIE
    } else if( owner -> age > owner -> deathage )
    {
        owner -> die( owner );
        return;
    }
}

void Actor::die( std::shared_ptr<Actor> owner )
{
    owner -> symbol = '~';
	owner -> col = TCODColor::red;	
	owner -> name = "Dead " + owner -> name;
    owner -> ai.reset();
}

void Actor::update( std::shared_ptr<Actor> owner )
{
    if( owner && ai )
    {
        ai -> update( owner );
    }

    if( owner )
    {
        owner -> age += 1;
    }
}

void Actor::render()
{
    engine.map -> map -> setChar( x, y, symbol );
    engine.map -> map -> setCharForeground( x, y, col );
}

// Reads a JSON file and constructs a creature from it.
bool Actor::readFile( std::string filename, std::shared_ptr<Actor> actor )
{
    // Open a file stream based on a file in the data folder
    std::ifstream ifs( "../data/" + filename );
    if( !ifs )
    {
        return false;
    } else
    {
        nlohmann::json file = nlohmann::json::parse( ifs );
        ifs.close();

        // The actor calls construction
        actor -> from_json( file );
        
        return true;
    }
}

// Outputs actor information into a JSON file.
void Actor::to_json( nlohmann::json& j )
{
    j = nlohmann::json
    {
        { "name", name },
        { "symbol", std::string( 1, symbol ) },
        { "color", 
            { "red", col.r },
            { "green", col.g },
            { "blue", col.b }
        },
        { "matingage", matingage },
        { "deathage", deathage }
    };
}

// Constructs an actor from a JSON file.
void Actor::from_json( nlohmann::json& j )
{
    name = j.at( "name" ).get<std::string>();

    std::string temp = j.at( "symbol" ).get<std::string>();
    symbol = temp[0];

    col.r = j.at( "color" ).at( "red" ).get<int>();
    col.g = j.at( "color" ).at( "green" ).get<int>();
    col.b = j.at( "color" ).at( "blue" ).get<int>();

    matingage = j.at( "matingage" ).get<int>();
    deathage = j.at( "deathage" ).get<int>();

    // Leg and foot are separate, but are made at the same time in the construction phase.
    for( int i = 0; i < j.at( "body" ).at( "leg" ).at( "count" ).get<int>(); ++i )
    {
        std::shared_ptr<Part> leg = std::make_shared<Part>();
        std::shared_ptr<Part> foot = std::make_shared<Part>();

        leg -> name = j.at( "body" ).at( "leg" ).at( "name" ).get<std::string>();
        leg -> length = j.at( "body" ).at( "leg" ).at( "length" ).get<int>();
        leg -> weight = j.at( "body" ).at( "leg" ).at( "weight" ).get<int>();
        for( int k = 0; k < j.at( "body" ).at( "leg" ).at( "function" ).size(); ++k )
        {
            leg -> function.push_back( static_cast<Part::Function>( j.at( "body" ).at( "leg" ).at( "function" ).at( k ).get<int>() ) );
        }

        foot -> name = j.at( "body" ).at( "leg" ).at( "foot" ).at( "name" ).get<std::string>();
        foot -> length = j.at( "body" ).at( "leg" ).at( "foot" ).at( "length" ).get<int>();
        foot -> weight = j.at( "body" ).at( "leg" ).at( "foot" ).at( "weight" ).get<int>();
        for( int k = 0; k < j.at( "body" ).at( "leg" ).at( "foot" ).at( "function" ).size(); ++k )
        {
            leg -> function.push_back( static_cast<Part::Function>( j.at( "body" ).at( "leg" ).at( "foot" ).at( "function" ).at( k ).get<int>() ) );
        }

        leg -> parts.push_back( foot );
        body -> parts.push_back( leg );
    }

    

    // Loops through all heads and gets relevant data.
    for( int i = 0; i < j.at( "body" ).at( "head" ).at( "count" ).get<int>(); ++i )
    {
        std::shared_ptr<Part> head = std::make_shared<Part>();

        head -> name = j.at( "body" ).at( "head" ).at( "name" ).get<std::string>();
        head -> length = j.at( "body" ).at( "head" ).at( "length" ).get<int>();
        head -> weight = j.at( "body" ).at( "head" ).at( "weight" ).get<int>();
        // Gets the functions of the item.
        for( int k = 0; k < j.at( "body" ).at( "head" ).at( "function" ).size(); ++k )
        {
            head -> function.push_back( static_cast<Part::Function>( j.at( "body" ).at( "head" ).at( "function" ).at( k ).get<int>() ) );
        }
    }
}