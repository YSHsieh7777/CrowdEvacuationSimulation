#include "simulator/fire.hpp"

Fire::Fire(float x, float y , float r)
{
    m_x = x;
    m_y = y;
    m_radius = r;
    m_speed = 0.1;
}

const float & Fire::x() const { return m_x; }
float & Fire::x() { return m_x; }
const float & Fire::y() const { return m_y; }
float & Fire::y() { return m_y; }
const float & Fire::r() const { return m_radius; }
float & Fire::r() { return m_radius; }

void Fire::update_radius()
{
    m_radius += m_speed;
}

void Fire::setColor(SDL_Renderer* gRenderer)
{
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0xFF);
}

void Fire::render(SDL_Renderer* gRenderer)
{
    Fire::setColor(gRenderer);

    const float diameter = (m_radius * 2);

    float x = (m_radius - 1);
    float y = 0;
    float tx = 1;
    float ty = 1;
    float error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(gRenderer, m_x + x, m_y - y);
        SDL_RenderDrawPoint(gRenderer, m_x + x, m_y + y);
        SDL_RenderDrawPoint(gRenderer, m_x - x, m_y - y);
        SDL_RenderDrawPoint(gRenderer, m_x - x, m_y + y);
        SDL_RenderDrawPoint(gRenderer, m_x + y, m_y - x);
        SDL_RenderDrawPoint(gRenderer, m_x + y, m_y + x);
        SDL_RenderDrawPoint(gRenderer, m_x - y, m_y - x);
        SDL_RenderDrawPoint(gRenderer, m_x - y, m_y + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}