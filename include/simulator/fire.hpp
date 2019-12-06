#include <utility>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <vector>
#include <stdlib.h>

class Fire
{
public:
    Fire(float x, float y , float r);
    ~Fire() = default;

    const float & x() const;
    float & x();
    const float & y() const;
    float & y();
    const float & r() const;
    float & r();

    void update_radius();

    void setColor(SDL_Renderer* );

    //Shows the Person on the screen
    void render(SDL_Renderer* );

private:
    float m_x, m_y;
    float m_radius;
    float m_speed;
};