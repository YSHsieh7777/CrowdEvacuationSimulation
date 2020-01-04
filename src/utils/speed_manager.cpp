#include "utils/speed_manager.hpp"

#include <stdlib.h>
#include <time.h>

SpeedManager::SpeedManager()
{
    srand(time(NULL));

    // Init a random average speed in a block
    // People in the block will move depending on that speed
    m_average_x = 0.4 * rand() / (RAND_MAX + 1.0) - 0.2;
    m_average_y = 0.4 * rand() / (RAND_MAX + 1.0) - 0.2;   
}

void SpeedManager::reset_speed(Person *cur_person)
{
    cur_person->x_speed() = 0;
    cur_person->y_speed() = 0;
}

void SpeedManager::set_speed(Person *cur_person, float x_speed, float y_speed)
{
    cur_person->x_speed() = x_speed;
    cur_person->y_speed() = y_speed;
}

void SpeedManager::set_next_speed(Person *cur_person, float x_speed, float y_speed)
{
    cur_person->x_next_speed() = x_speed;
    cur_person->y_next_speed() = y_speed;
}

void SpeedManager::set_stay_away_speed(Person *p1, Person *p2, float x_speed, float y_speed)
{
    set_next_speed(p1, p1->x_next_speed() + x_speed * 0.007, p1->y_next_speed() + y_speed * 0.007);
    set_next_speed(p2, p2->x_next_speed() + -(x_speed) * 0.007, p2->y_next_speed() + -(y_speed) * 0.007);
}

void SpeedManager::update_speed_in_door(Person *cur_person)
{
    // If the people are passing the door, don't give them other speed
    // So they can pass the door smoothly
    set_speed(cur_person, cur_person->x_next_speed(), cur_person->y_next_speed());
}

void SpeedManager::update_speed_by_panic_degree(Person * cur_person, float x_speed_to_door, float y_speed_to_door)
{
    if(cur_person->panic_degree() == 5)
    {
        set_speed(cur_person, 
                  0.2 * rand() / (RAND_MAX + 1.0) - 0.1 + 0.03 * x_speed_to_door, 
                  0.2 * rand() / (RAND_MAX + 1.0) - 0.1 + 0.03 * y_speed_to_door);
    }
    else if(cur_person->panic_degree() == 4)
    {
        set_speed(cur_person, 
                  cur_person->x_next_speed() + 0.02 * x_speed_to_door, 
                  cur_person->y_next_speed() + 0.02 * y_speed_to_door);
    }
    else if(cur_person->panic_degree() == 3)
    {
        // set_speed(cur_person, cur_person->x_next_speed() + 0.01 * m_average_x + 0.01 * x_speed_to_door, cur_person->y_next_speed() + 0.01 * m_average_y + 0.007 * y_speed_to_door);
        set_speed(cur_person, 
                  cur_person->x_next_speed() + 0.007 * x_speed_to_door + 0.01 * m_average_x, 
                  cur_person->y_next_speed() + 0.007 * y_speed_to_door + 0.01 * m_average_y);

    }
    else if(cur_person->panic_degree() == 2)
    {
        // set_speed(cur_person, cur_person->x_next_speed() + 0.025 * m_average_x + 0.005 * x_speed_to_door, cur_person->y_next_speed() + 0.025 * m_average_y + 0.004 * y_speed_to_door);
        set_speed(cur_person, 
                  cur_person->x_next_speed() + 0.004 * x_speed_to_door + 0.025 * m_average_x, 
                  cur_person->y_next_speed() + 0.004 * y_speed_to_door + 0.025 * m_average_y);

    }
    else if(cur_person->panic_degree() == 1)
    {
        // set_speed(cur_person, cur_person->x_next_speed() + 0.04 * m_average_x + 0.001 * x_speed_to_door, cur_person->y_next_speed() + 0.04 * m_average_y + 0.001 * y_speed_to_door);
        set_speed(cur_person, 
                  cur_person->x_next_speed() + 0.001 * x_speed_to_door + 0.04 * m_average_x, 
                  cur_person->y_next_speed() + 0.001 * y_speed_to_door + 0.04 * m_average_y);
    }
    else
    {
        // set_speed(cur_person, cur_person->x_next_speed() + 0.05 * m_average_x, cur_person->y_next_speed() + 0.05 * m_average_y);
        set_speed(cur_person, 
                  cur_person->x_next_speed() + 0.05 * rand() / (RAND_MAX + 1.0) - 0.0125 + 0.05 * m_average_x, 
                  cur_person->y_next_speed() + 0.05 * rand() / (RAND_MAX + 1.0) - 0.0125 + 0.05 * m_average_y);
    }
}

void SpeedManager::update_block_speed(Person * cur_person)
{
    // update the average moving speed in a block
    m_average_x = 0.95 * m_average_x + 0.05 * cur_person->x_speed();
    m_average_y = 0.95 * m_average_y + 0.05 * cur_person->y_speed();
}