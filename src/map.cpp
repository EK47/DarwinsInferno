#include "main.hpp"

Chunk::Chunk()
{

}

Chunk::~Chunk()
{

}

Map::Map( int width, int height ) : width( width ), height( height )
{
    // Creates a console
    map = std::make_shared<TCODConsole>( width, height );
    // Creates a walkable terrain map.
    mapMap = std::make_shared<TCODMap>( width, height );
    // Creates a path based on the map.
    mapPath = std::make_shared<TCODPath>( mapMap.get() );
    // Creates a heightmap the size of the map
    heightMap = std::make_shared<TCODHeightMap>( width, height );
    // Creates noise, for generating the terrain.
    terrainGen = std::make_shared<TCODNoise>( 2 );
    terrainGen -> setType( TCOD_NOISE_SIMPLEX );

    regenerate();
}

Map::~Map()
{

}

bool Map::regenerate()
{
    std::shared_ptr<TCODHeightMap> heightMap2 = std::make_shared<TCODHeightMap>( width, height );
    heightMap -> clear();

    // This crazy stuff makes the map
    heightMap -> addFbm( terrainGen.get(), 1, 1, 0.0, 0.0, 20, 0.0, 1.0 );
    heightMap2 -> addFbm( terrainGen.get(), 2, 2, 0.0, 0.0, 10, 0.0, 1.0 );

    heightMap -> multiply( heightMap.get(), heightMap2.get() );
    heightMap -> normalize( 0.0f, 1.0f );

    // I have to implement a custom version of the erosion system, since the libtcod one segfaults when used.
    int nbDrops = width * height * 2.5;
    float erosionCoef = 0.75f;
    float agregationCoef = 0.01f;

    static int dx[8]={ -1, 0, 1, -1, 1, -1, 0, 1 };
	static int dy[8]={ -1, -1, -1, 0, 0, 1, 1, 1 };
    static float weight[8] = { .125, .125, .125, .125, .125, .125, .125, .125, };

    while( nbDrops > 0 )
    {
        int curx = rand() % width;
        int cury = rand() % height;

        float slope = 0.0f;

        float sediment = 0.0f;

        do
        {
            int nextx = 0, nexty = 0, i;

            float v = heightMap -> getValue( curx, cury );

            slope = 0.0f;

            for ( i = 0; i < 8; i++ )
            {
				int nx = curx + dx[i];
				int ny = cury + dy[i];
				if ( nx >= 0 && nx < width && ny >= 0 && ny < height ) {
					float nslope = v - heightMap -> getValue( nx, ny );
					if ( nslope > slope )
                    {
						slope = nslope;
						nextx = nx;
						nexty = ny;
					}
				}
			}

            if ( slope > 0.0f )
            {
                heightMap -> setValue( curx, cury, heightMap -> getValue( curx, cury ) * 1.0f - erosionCoef * slope );
				//heightMap -> setValue( curx, cury, heightMap -> getValue( curx, cury ) - erosionCoef * slope );
				curx = nextx;
				cury = nexty;
				sediment += slope;
			} else {
                heightMap -> setValue( curx, cury, heightMap -> getValue( curx, cury ) * 1.0f + agregationCoef * slope );
				//heightMap -> setValue( curx, cury, heightMap -> getValue( curx, cury ) + agregationCoef * sediment );
			}
        } while( slope > 0.0f );
        nbDrops--;
    }

    // Blend everything.
    heightMap -> kernelTransform( 8, dx, dy, weight, 0.0f, 1.0f );

    // Renormalize to make sure all values are within the range of rendering.
    heightMap -> normalize( 0.0f, 1.0f );

    // This generates rivers.

    int BezierPX[4] = { 40, 19, 76, 50 };
    int BezierPY[4] = { height, height / 2, height / 2, 0 };

    heightMap -> digBezier( BezierPX, BezierPY, 0.5, 0.4, 0.5, 0.4 );

    // Creates a map of where creatures can walk.
    
    for( int x = 0; x < width; ++x )
    {
        for( int y = 0; y < height; ++y )
        {
            if( heightMap -> getValue( x, y ) > 0.15f )
            {
                mapMap -> setProperties( x, y, true, true );
            } else
            {
                mapMap -> setProperties( x, y, true, true );
            }
        }
    }

    // Generates creatures and places them in random places.
    for( int i = 0; i < 20; ++i )
    {
        std::shared_ptr<Actor> actor = std::make_shared<Actor>( '0', "0", TCODColor::black );

        actor -> readFile( "Wolf.json", actor );

        actor -> x = rand() % width;
        actor -> y = rand() % height;

        engine.actors.push_back( actor );
    }

    return true;
}

void Map::render() const
{
    map -> clear();
    for( int y = 0; y < height; y++ )
    {
        for( int x = 0; x < width; x++ )
        {
            TCODColor tile;
            if( heightMap -> getValue( x, y ) < 0.15f )
            {
                tile = TCODColor::lerp( TCODColor::darkestBlue, TCODColor::green, heightMap -> getValue( x, y ) );
            } else if( heightMap -> getValue( x, y ) < 0.8f || heightMap -> getValue( x, y ) > 0.15f )
            {
                tile = TCODColor::green * heightMap -> getValue( x, y );
            } else if( heightMap -> getValue( x, y ) > 0.8f )
            {
                tile = TCODColor::lerp( TCODColor::darkerGreen, TCODColor::lightestGrey, heightMap -> getValue( x, y ) );
            }

            map -> setCharBackground( x, y, tile );
        }
    }
}