#include "object/person.hpp"

#include <map>

std::map<uint16_t, uint16_t> panic_to_red = {{0, 0}, {1, 50}, {2, 100}, {3, 150}, {4, 200}, {5, 255}};

Person::Person(float x, float y , float r)
{
    m_x = x;
    m_y = y;
    m_radius = r;
    m_x_speed = rand() / (RAND_MAX + 1.0) - 0.5;
    m_y_speed = rand() / (RAND_MAX + 1.0) - 0.5;
    m_x_next_speed = m_y_speed;
    m_y_next_speed = m_y_speed;

    m_panic_degree = 0;
    m_moving_distance_last_few_seconds = 0;  // A factor that affects panic_degree.
    m_dead = false;  
    m_pass_door = false;
}

const float & Person::x() const { return m_x; }
float & Person::x() { return m_x; }
const float & Person::y() const { return m_y; }
float & Person::y() { return m_y; }
const float & Person::r() const { return m_radius; }
float & Person::r() { return m_radius; }
const float & Person::x_speed() const { return m_x_speed; }
float & Person::x_speed() { return m_x_speed; }
const float & Person::y_speed() const { return m_y_speed; }
float & Person::y_speed() { return m_y_speed; }
const float & Person::x_next_speed() const { return m_x_next_speed; }
float & Person::x_next_speed() { return m_x_next_speed; }
const float & Person::y_next_speed() const { return m_y_next_speed; }
float & Person::y_next_speed() { return m_y_next_speed; }
const bool & Person::pass_door() const { return m_pass_door; }
bool & Person::pass_door() { return m_pass_door; }
const bool & Person::is_dead() const { return m_dead; }
bool & Person::is_dead() { return m_dead; }
const uint16_t & Person::panic_degree() const { return m_panic_degree; }
uint16_t & Person::panic_degree() { return m_panic_degree; }

void Person::move()
{
    //Move the Person left or right
    m_x += m_x_speed;

    //Move the Person up or down
    m_y += m_y_speed;
}

void Person::setColor(SDL_Renderer* gRenderer)
{
    SDL_SetRenderDrawColor(gRenderer, panic_to_red[m_panic_degree], 0, 0, 0xFF);
}

void Person::render(SDL_Renderer* gRenderer)
{
    Person::setColor(gRenderer);

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