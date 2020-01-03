#include "object/object.hpp"

#include <SDL2/SDL.h>

class Fire final : public Object 
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

    //Shows the fire on the screen
    void render(SDL_Renderer* ) override;

private:
    float m_x, m_y;
    float m_radius;
    float m_speed;
};