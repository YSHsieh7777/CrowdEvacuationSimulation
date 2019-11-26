#include "person.hpp"

std::map<uint8_t, uint8_t> panic_to_red = {{0, 0}, {1, 255}};
std::map<uint8_t, uint8_t> panic_to_green = {{0, 0}, {1, 0}};
std::map<uint8_t, uint8_t> panic_to_blue = {{0, 0}, {1, 0}};

Person::Person(float x, float y , float r)
{
    m_x = x;
    m_y = y;
	m_radius = r;
	m_x_speed = rand() / (RAND_MAX + 1.0) - 0.5;
    m_y_speed = rand() / (RAND_MAX + 1.0) - 0.5;

    m_panic_degree = 0;
	m_moving_distance_last_few_seconds = 0;  // A factor that affects panic_degree.
    m_dead = false;  
}

const float & Person::x() const { return m_x; }
float & Person::x() { return m_x; }
const float & Person::y() const { return m_y; }
float & Person::y() { return m_y; }
const float & Person::x_speed() const { return m_x_speed; }
float & Person::x_speed() { return m_x_speed; }
const float & Person::y_speed() const { return m_y_speed; }
float & Person::y_speed() { return m_y_speed; }

void Person::move()
{
    //Move the Person left or right
    m_x += m_x_speed;

    //Move the Person up or down
    m_y += m_y_speed;
}

void Person::setColor(SDL_Renderer* gRenderer)
{
    SDL_SetRenderDrawColor(gRenderer, panic_to_red[m_panic_degree], panic_to_green[m_panic_degree], panic_to_blue[m_panic_degree], 0xFF);
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