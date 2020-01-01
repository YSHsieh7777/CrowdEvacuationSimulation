#include "object/person.hpp"

class SpeedManager
{
public:
    SpeedManager();
    ~SpeedManager() = default;

    void reset_speed(Person * );
    void set_speed(Person * , float , float );
    void set_next_speed(Person * , float , float );
    void set_stay_away_speed(Person * , Person * , float , float );
    void update_speed_in_door(Person * );
    void update_speed_by_panic_degree(Person * , float , float );
    void update_block_speed(Person * );

private:
    float m_average_x, m_average_y;
};