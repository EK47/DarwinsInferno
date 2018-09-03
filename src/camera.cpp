#include "main.hpp"

Camera::Camera( int width, int height ) : cWidth( width ), cHeight( height )
{
    camera = std::make_shared<TCODConsole>( width, height );

    cChunkX = -1;
    cChunkY = -1;
}

Camera::~Camera()
{

}

void Camera::render()
{
    camera -> clear();
    TCOD_key_t key = TCODConsole::waitForKeypress( true );

    switch( key.vk )
    {
        case TCODK_LEFT: cChunkX -= 1; break;
        case TCODK_DOWN: cChunkY += 1; break;
        case TCODK_UP: cChunkY -= 1; break;
        case TCODK_RIGHT: cChunkX += 1; break;
        default: break;
    }

    // Makes sure the camera cannot go beyond the bounds of the map. The top left edge is calculated by subtracting 
    // map width by the width of the camera. Those two values are divided by sixteen ( size of a chunk ), and then by one,
    // since it starts at zero.
    cChunkX = std::max( -1, std::min( cChunkX, ( engine.map -> width / 16 ) - ( cWidth / 16 ) - 1 ) );
    cChunkY = std::max( -1, std::min( cChunkY, ( engine.map -> height / 16 ) - ( cHeight / 16 ) - 1 ) );

    TCODConsole::blit( engine.map -> map.get(), ( cChunkX + 1 ) * 16, ( cChunkY + 1 ) * 16, 128, 112, camera.get(), 0, 0 );
    TCODConsole::blit( camera.get(), 0, 0, 128, 112, TCODConsole::root, 0, 0 );
}