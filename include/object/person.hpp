#include "object/object.hpp"

#include <stdint.h>
#include <SDL2/SDL.h>

class Person final : Object
{
public:
    Person(float , float , float );
    ~Person() = default;

    const float & x() const;
    float & x();
    const float & y() const;
    float & y();
    const float & r() const;
    float & r();
    const float & x_speed() const;
    float & x_speed();
    const float & y_speed() const;
    float & y_speed();
    const float & x_next_speed() const;
    float & x_next_speed();
    const float & y_next_speed() const;
    float & y_next_speed();
    const bool & pass_door() const;
    bool & pass_door();
    const bool & is_dead() const;
    bool & is_dead();
    const uint16_t & panic_degree() const;
    uint16_t & panic_degree();

    void move();

    //Shows the Person on the screen
    void render(SDL_Renderer* ) override;

private:
    float m_x, m_y;
    float m_radius;
    float m_x_speed, m_y_speed;
    float m_x_next_speed, m_y_next_speed;

    uint16_t m_panic_degree;
    bool m_update_fire_distance;
    float m_moving_distance_last_few_seconds;  // A factor that affects panic_degree.
    bool m_dead;
    bool m_pass_door;
};