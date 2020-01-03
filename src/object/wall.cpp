#include "object/wall.hpp"

Wall::Wall()
{
    m_walls = std::vector<SDL_Rect>(12);

    // Draw the walls in a brute way
    m_walls[0].x = 50;
    m_walls[0].y = 50;
    m_walls[0].w = 490;
    m_walls[0].h = 30;

    m_walls[1].x = 50;
    m_walls[1].y = 280;
    m_walls[1].w = 110;
    m_walls[1].h = 30;

    m_walls[2].x = 200;
    m_walls[2].y = 280;
    m_walls[2].w = 190;
    m_walls[2].h = 30;

    m_walls[3].x = 430;
    m_walls[3].y = 280;
    m_walls[3].w = 110;
    m_walls[3].h = 30;

    m_walls[4].x = 50;
    m_walls[4].y = 510;
    m_walls[4].w = 340;
    m_walls[4].h = 30;

    m_walls[5].x = 430;
    m_walls[5].y = 510;
    m_walls[5].w = 110;
    m_walls[5].h = 30;

    m_walls[6].x = 50;
    m_walls[6].y = 50;
    m_walls[6].w = 30;
    m_walls[6].h = 110;

    m_walls[7].x = 50;
    m_walls[7].y = 200;
    m_walls[7].w = 30;
    m_walls[7].h = 340;

    m_walls[8].x = 280;
    m_walls[8].y = 50;
    m_walls[8].w = 30;
    m_walls[8].h = 110;

    m_walls[9].x = 280;
    m_walls[9].y = 200;
    m_walls[9].w = 30;
    m_walls[9].h = 190;

    m_walls[10].x = 280;
    m_walls[10].y = 430;
    m_walls[10].w = 30;
    m_walls[10].h = 110;

    m_walls[11].x = 510;
    m_walls[11].y = 50;
    m_walls[11].w = 30;
    m_walls[11].h = 490;
}

void Wall::render(SDL_Renderer* gRenderer)
{
    setColor(gRenderer, 0, 0, 0);

    // Render wall 
    for(int i=0; i<12; i++)
    {
        SDL_RenderDrawRect(gRenderer, &m_walls[i] );
    }    
}