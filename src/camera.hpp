#ifndef camera_hpp
#define camera_hpp
#pragma once

class Camera
{
public:
    Camera( int width, int height );
    ~Camera();

    void render();

    int cChunkX, cChunkY;
    int cWidth, cHeight;
    std::shared_ptr<TCODConsole> camera;
};

#endif