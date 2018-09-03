#ifndef map_hpp
#define map_hpp
#pragma once

// A 16x16 area of information. Chunks are much more manageable than an entire map.
class Chunk
{
    Chunk();
    ~Chunk();
};

// The map. Contains all beings and is the main environment viewed by the overseer.
class Map
{
public:
    Map( int width, int height );
    ~Map();
    bool regenerate();
    void render() const;

    int width, height;

    std::vector<std::shared_ptr<Chunk>> chunks;

    std::shared_ptr<TCODHeightMap> heightMap;
    std::shared_ptr<TCODNoise> terrainGen;
    std::shared_ptr<TCODConsole> map;
    std::shared_ptr<TCODMap> mapMap;
    std::shared_ptr<TCODPath> mapPath;
private:
};

#endif