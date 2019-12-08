#include "simulator/map_block.hpp"

MapBlock::MapBlock(size_t left_bound, size_t right_bound, size_t up_bound, 
                        size_t bottom_bound, size_t x_people_num, size_t y_people_num,
                        bool left_door, bool right_door, bool up_door, 
                        bool bottom_door): 
                        m_left_bound(left_bound), m_right_bound(right_bound),
                        m_up_bound(up_bound), m_bottom_bound(bottom_bound),
                        m_left_door(left_door), m_right_door(right_door),
                        m_up_door(up_door), m_bottom_door(bottom_door) 
{
    m_people_num = x_people_num * y_people_num;

    for(uint8_t y=1; y<y_people_num+1; ++y)
    {
        for(uint8_t x=1; x<x_people_num+1; ++x)
        {    
            // Init a person every 25 pixels and give them a random position in that block
            float rand_x_pos = x*25 + (17.5 - 7.5) * rand() / (RAND_MAX + 1.0) + 7.5;
            float rand_y_pos = y*25 + (17.5 - 7.5) * rand() / (RAND_MAX + 1.0) + 7.5;
            
            Person *p = new Person(m_left_bound + rand_x_pos, m_up_bound + rand_y_pos, 7.5);
            m_people.push_back(p);
        }
    }

    // Init a random average speed in a block
    // People in the block will move depending on that speed
    m_average_x = 0.4 * rand() / (RAND_MAX + 1.0) - 0.2;
    m_average_y = 0.4 * rand() / (RAND_MAX + 1.0) - 0.2;        
}

MapBlock::~MapBlock()
{
    for(size_t i=0; i<m_people_num; ++i)
    {
        delete m_people[i];
    }
    for(size_t i=0; i<m_dead_people.size(); ++i)
    {
        delete m_dead_people[i];
    }
}

void MapBlock::add_neighbors(MapBlock* l_neighbor, MapBlock* r_neighbor, MapBlock* u_neighbor, MapBlock* d_neighbor)
{
    m_l_neighbor = l_neighbor;
    m_r_neighbor = r_neighbor;
    m_u_neighbor = u_neighbor;
    m_d_neighbor = d_neighbor;
}

const size_t & MapBlock::people_num() const { return m_people_num; }
size_t & MapBlock::people_num() { return m_people_num; }
const std::vector<Person* > & MapBlock::people() const { return m_people; }
std::vector<Person *> & MapBlock::people() { return m_people; }

void MapBlock::update_people_speed()
{    
    for(size_t i=0; i<m_people_num; ++i)
    {    
        if(m_people[i]->pass_door())
        {
            // If the people are passing the door, don't give them other speed
            // So they can pass the door smoothly
            m_people[i]->x_speed() = m_people[i]->x_next_speed();
            m_people[i]->y_speed() = m_people[i]->y_next_speed();
        }    
        else 
        {
            m_people[i]->x_speed() = m_people[i]->x_next_speed() + 0.5 * m_average_x;
            m_people[i]->y_speed() = m_people[i]->y_next_speed() + 0.5 * m_average_y;
        }    

        // Update the average moving speed in a block
        m_average_x = 0.95 * m_average_x + 0.05 * m_people[i]->x_speed();
        m_average_y = 0.95 * m_average_y + 0.05 * m_people[i]->y_speed();
    }

    // std::cout << "average x: " << m_average_x << std::endl;
    // std::cout << "average y: " << m_average_y << std::endl;
}

void MapBlock::check_walls_collision()
{    
    std::vector<size_t> erase_id;

    for(size_t i=0; i<m_people_num; ++i)
    {
        float move_x_pos = m_people[i]->x() + m_people[i]->x_speed();
        float move_y_pos = m_people[i]->y() + m_people[i]->y_speed();
        float person_radius = m_people[i]->r();
        float move_left_bound = move_x_pos - person_radius;
        float move_right_bound = move_x_pos + person_radius;
        float move_up_bound = move_y_pos - person_radius;
        float move_bottom_bound = move_y_pos + person_radius;

        // If there's a door on left wall
        if(m_left_door)
        {
            // If the person will move into the wall, move him back.
            if(move_left_bound < m_left_bound && (move_up_bound < (m_up_bound+90) || move_bottom_bound > (m_bottom_bound-90)))
            {
                m_people[i]->x_speed() =  -(m_people[i]->x() - m_left_bound - person_radius);
                m_people[i]->x_next_speed() = 0.2;
            }
            // If the person will move into the door, give him a speed so that
            // he can pass the door fast
            else if(move_left_bound < m_left_bound && (move_up_bound >= (m_up_bound+90) || move_bottom_bound <= (m_bottom_bound-90)))
            {
                if(!m_people[i]->pass_door() && m_people[i]->x_speed() < 0)
                {
                    m_people[i]->x_next_speed() = -0.5;
                    m_people[i]->y_next_speed() = 0;
                    m_people[i]->pass_door() = true;
                }
            }
        }
        else
        {
            // If the person will move into the wall, move him back.
            if(move_left_bound < m_left_bound)
            {
                m_people[i]->x_speed() =  -(m_people[i]->x() - m_left_bound - person_radius);
                m_people[i]->x_next_speed() = 0.2;
            }
        }
        
        if(m_right_door)
        {    
            if(move_right_bound > m_right_bound && (move_up_bound < (m_up_bound+90) || move_bottom_bound > (m_bottom_bound-90)))
            {
                m_people[i]->x_speed() =  m_right_bound - m_people[i]->x() - person_radius;
                m_people[i]->x_next_speed() = -0.2;
            }
            else if(move_right_bound > m_right_bound && (move_up_bound >= (m_up_bound+90) || move_bottom_bound <= (m_bottom_bound-90)))
            {
                if(!m_people[i]->pass_door() && m_people[i]->x_speed() > 0)
                {
                    m_people[i]->x_next_speed() = 0.5;
                    m_people[i]->y_next_speed() = 0;
                    m_people[i]->pass_door() = true;
                }
            }
        }
        else
        {
            if(move_right_bound > m_right_bound)
            {
                m_people[i]->x_speed() = m_right_bound - m_people[i]->x() - person_radius;
                m_people[i]->x_next_speed() = -0.2;
            }
        }
        
        if(m_up_door)
        {
            if(move_up_bound < m_up_bound && (move_left_bound < (m_left_bound+90) || move_right_bound > (m_right_bound-90)))
            {
                m_people[i]->y_speed() = -(m_people[i]->y() - m_up_bound - person_radius);
                m_people[i]->y_next_speed() = 0.2;
            }
            else if(move_up_bound < m_up_bound && (move_left_bound >= (m_left_bound+90) || move_right_bound <= (m_right_bound-90)))
            {
                if(!m_people[i]->pass_door() && m_people[i]->y_speed() < 0)
                {
                    m_people[i]->y_next_speed() = -0.5;
                    m_people[i]->x_next_speed() = 0;
                    m_people[i]->pass_door() = true;
                }
            }
        }
        else
        {
            if(move_up_bound < m_up_bound)
            {
                m_people[i]->y_speed() = -(m_people[i]->y() - m_up_bound - person_radius);
                m_people[i]->y_next_speed() = 0.1;
            }
        }
        
        if(m_bottom_door)
        {
            if(move_bottom_bound > m_bottom_bound && (move_left_bound < (m_left_bound+90) || move_right_bound > (m_right_bound-90)))
            {
                m_people[i]->y_speed() = m_bottom_bound - m_people[i]->y() - person_radius;
                m_people[i]->y_next_speed() = -0.2;
            }
            else if(move_bottom_bound > m_bottom_bound && (move_left_bound >= (m_left_bound+90) || move_right_bound <= (m_right_bound-90)))
            {
                if(!m_people[i]->pass_door() && m_people[i]->y_speed() > 0)
                {
                    m_people[i]->y_next_speed() = 0.5;
                    m_people[i]->x_next_speed() = 0;
                    m_people[i]->pass_door() = true;
                }
            }
        }
        else
        {
            if(move_bottom_bound > m_bottom_bound)
            {
                m_people[i]->y_speed() = m_bottom_bound - m_people[i]->y() - person_radius;
                m_people[i]->y_next_speed() = -0.2;
            }
        }

        // If there's a neighbor block on right side,
        // and there's a person moving into that block,
        // push the person to that block and delete him in this block
        if(m_r_neighbor != NULL)
        {
            if(move_right_bound > (m_right_bound+30))
            {
                m_people[i]->pass_door() = false;
                m_r_neighbor->people().push_back(m_people[i]);
                m_r_neighbor->people_num() += 1;
                erase_id.push_back(i);
            }
        }
        else
        {    
            if(move_right_bound > (m_right_bound+30))
            {
                m_people[i]->pass_door() = false;
                erase_id.push_back(i);
            }
        }
            
        if(m_l_neighbor != NULL)
        {
            if(move_left_bound < (m_left_bound-30))
            {
                m_people[i]->pass_door() = false;
                m_l_neighbor->people().push_back(m_people[i]);
                m_l_neighbor->people_num() += 1;
                erase_id.push_back(i);
            }
        }
        else
        {
            if(move_left_bound < (m_left_bound-30))
            {
                m_people[i]->pass_door() = false;
                erase_id.push_back(i);
            }
        }

        if(m_u_neighbor != NULL)
        {
            if(move_up_bound < (m_up_bound-30))
            {
                m_people[i]->pass_door() = false;
                m_u_neighbor->people().push_back(m_people[i]);
                m_u_neighbor->people_num() += 1;
                erase_id.push_back(i);
            }
        }
        else
        {
            if(move_up_bound < (m_up_bound-30))
            {
                m_people[i]->pass_door() = false;
                erase_id.push_back(i);
            }
        }

        if(m_d_neighbor != NULL)
        {
            if(move_bottom_bound > (m_bottom_bound+30))
            {
                m_people[i]->pass_door() = false;
                m_d_neighbor->people().push_back(m_people[i]);
                m_d_neighbor->people_num() += 1;
                erase_id.push_back(i);
            }
        }
        else
        {
            if(move_bottom_bound > (m_bottom_bound+30))
            {
                m_people[i]->pass_door() = false;
                erase_id.push_back(i);
            }
        }
    }

    // Delete people who move to other block
    if(!erase_id.empty())
    {
        for(size_t i=0; i<erase_id.size(); ++i)
        {
            m_people[erase_id[i]] = m_people.back();
            m_people.pop_back();
            m_people_num -= 1;
        }
    }
}

void MapBlock::check_person_collision(size_t num)
{
    float cur_move_x_pos = m_people[num]->x() + m_people[num]->x_speed();
    float cur_move_y_pos = m_people[num]->y() + m_people[num]->y_speed();

    for(size_t i=0; i<m_people_num; ++i)
    {
        if(m_people[i]->pass_door())
            continue;

        float move_x_pos = m_people[i]->x() + m_people[i]->x_speed();
        float move_y_pos = m_people[i]->y() + m_people[i]->y_speed();

        if(i == num)
            continue;
        else
        {
            // If there's a collision, make the people stop, and give them a speed
            // to move away from each other. Then check the collision again.
            float distance = (cur_move_x_pos-move_x_pos) * (cur_move_x_pos-move_x_pos) + (cur_move_y_pos-move_y_pos) * (cur_move_y_pos-move_y_pos);
            float collision_bound = (2 * m_people[i]->r()) * (2 * m_people[i]->r());
            if(distance < collision_bound)
            {
                m_people[num]->x_speed() = 0;
                m_people[num]->y_speed() = 0;
                m_people[i]->x_speed() = 0;
                m_people[i]->y_speed() = 0;
                m_people[num]->x_next_speed() = m_people[num]->x_next_speed() + (cur_move_x_pos-move_x_pos) * 0.01;
                m_people[num]->y_next_speed() = m_people[num]->y_next_speed() + (cur_move_y_pos-move_y_pos) * 0.01;
                m_people[i]->x_next_speed() = m_people[i]->x_next_speed() + (move_x_pos-cur_move_x_pos) * 0.01;
                m_people[i]->y_next_speed() = m_people[i]->y_next_speed() + (move_y_pos-cur_move_y_pos) * 0.01;
                check_person_collision(i);
                check_person_collision(num);
                break;
            }
        }    
    }
}

void MapBlock::check_people_collision()
{
    for(size_t i=0; i<m_people_num; ++i)
    {
        // If the person is passing the door, don't check the collision
        // If checking the collision when the person is getting out of the door, there's a bug
        // This is a bug to fix
        if(!m_people[i]->pass_door())
        {
            check_person_collision(i);
        }     
    }
}

void MapBlock::check_fire_collision(size_t x, size_t y, size_t r, size_t fire_id)
{
    std::vector<size_t> erase_id;

    for(size_t i=0; i<m_people_num; ++i)
    {
        float move_x_pos = m_people[i]->x() + m_people[i]->x_speed();
        float move_y_pos = m_people[i]->y() + m_people[i]->y_speed();

        // If there's a collision, the person is dead
        float distance = sqrt((x-move_x_pos) * (x-move_x_pos) + (y-move_y_pos) * (y-move_y_pos));
        float collision_bound = m_people[i]->r() + r;
        if(distance < collision_bound)
        {
            m_people[i]->is_dead() = true;
            erase_id.push_back(i);
        }
        else
        {
            float gap = distance - collision_bound;
            if(gap < m_people[i]->fire_distance()[fire_id])
                m_people[i]->fire_distance()[fire_id] = gap;
            float min_distance = std::min(m_people[i]->fire_distance()[0], m_people[i]->fire_distance()[1]);

            if(min_distance < 30)
                m_people[i]->panic_degree() = 5;
            else if(min_distance >= 30 && min_distance < 60)
                m_people[i]->panic_degree() = 4;
            else if(min_distance >= 60 && min_distance < 90)
                m_people[i]->panic_degree() = 3;
            else if(min_distance >= 90 && min_distance < 120)
                m_people[i]->panic_degree() = 2;
            else if(min_distance >= 120 && min_distance < 150)
                m_people[i]->panic_degree() = 1;
        }
    }

    // Delete people who is dead
    if(!erase_id.empty())
    {
        for(size_t i=0; i<erase_id.size(); ++i)
        {
            m_dead_people.push_back(m_people[erase_id[i]]);
            m_people[erase_id[i]] = m_people.back();
            m_people.pop_back();
            m_people_num -= 1;
        }
    }
}

void MapBlock::people_move()
{
    for(size_t i=0; i<m_people_num; ++i)
    {
        m_people[i]->move();
    }
}

void MapBlock::update_map_block()
{   
    check_walls_collision();
    check_people_collision();
    people_move();
    update_people_speed();
}