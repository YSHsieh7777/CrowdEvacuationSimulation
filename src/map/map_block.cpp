#include "map/map_block.hpp"

#include <stdint.h>
#include <math.h>
#include <algorithm>
#include <limits>
#include <iostream>

MapBlock::MapBlock(size_t l_bound, size_t r_bound, size_t u_bound, 
                    size_t d_bound, size_t x_people_num, size_t y_people_num,
                    Door* l_door, Door* r_door, Door* u_door, 
                    Door* d_door, bool has_out_door): 
                    m_l_bound(l_bound), m_r_bound(r_bound),
                    m_u_bound(u_bound), m_d_bound(d_bound),
                    m_l_door(l_door), m_r_door(r_door),
                    m_u_door(u_door), m_d_door(d_door), m_has_out_door(has_out_door) 
{
    m_people_num = x_people_num * y_people_num;

    for(uint8_t y=1; y<y_people_num+1; ++y)
    {
        for(uint8_t x=1; x<x_people_num+1; ++x)
        {    
            // Init a person every 25 pixels and give him a random position in that block
            float rand_x_pos = x*25 + (17.5 - 7.5) * rand() / (RAND_MAX + 1.0) + 7.5;
            float rand_y_pos = y*25 + (17.5 - 7.5) * rand() / (RAND_MAX + 1.0) + 7.5;
            
            Person *p = new Person(m_l_bound + rand_x_pos, m_u_bound + rand_y_pos, 7.5);
            m_people.push_back(p);
        }
    }

    m_sm = new SpeedManager(); 
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

    delete m_l_door;
    delete m_r_door;
    delete m_u_door;
    delete m_d_door;
    delete m_sm;
}

const size_t & MapBlock::people_num() const { return m_people_num; }
size_t & MapBlock::people_num() { return m_people_num; }
const std::vector<Person* > & MapBlock::people() const { return m_people; }
std::vector<Person *> & MapBlock::people() { return m_people; }

void MapBlock::add_neighbors(MapBlock* l_neighbor, MapBlock* r_neighbor, MapBlock* u_neighbor, MapBlock* d_neighbor)
{
    m_l_neighbor = l_neighbor;
    m_r_neighbor = r_neighbor;
    m_u_neighbor = u_neighbor;
    m_d_neighbor = d_neighbor;
}

bool MapBlock::move_in_door(float move_l_bound, float move_r_bound, float door_l_bound, float door_r_bound)
{
    return (move_l_bound >= door_l_bound && move_r_bound <= door_r_bound);
}

void MapBlock::check_walls_collision(Person *cur_person, float person_radius, float move_l_bound, 
                                     float move_r_bound, float move_u_bound, float move_d_bound)
{    
    if(move_l_bound < m_l_bound)
    {
        // If there's a door on l wall
        // And the person will move into the door, give him a speed so that
        // he can pass the door fast 
        if(m_l_door && move_in_door(move_u_bound, move_d_bound, m_l_door->u_bound(), m_l_door->d_bound()))
        {      
            if(!cur_person->pass_door() && cur_person->x_speed() < 0)
            {
                m_sm->set_next_speed(cur_person, -0.5, 0);
                cur_person->pass_door() = true;
            }
        }
        else
        {
            // If the person will move into the wall, move him back.
            m_sm->set_speed(cur_person, -(cur_person->x() - m_l_bound - person_radius), cur_person->y_speed());
            m_sm->set_next_speed(cur_person, 0.2, cur_person->y_next_speed());
        }
    }
    
    if(move_r_bound > m_r_bound)
    {
        if(m_r_door && move_in_door(move_u_bound, move_d_bound, m_r_door->u_bound(), m_r_door->d_bound()))
        {    
            if(!cur_person->pass_door() && cur_person->x_speed() > 0)
            {
                m_sm->set_next_speed(cur_person, 0.5, 0);
                cur_person->pass_door() = true;
            }
        }
        else
        {
            m_sm->set_speed(cur_person, (m_r_bound - cur_person->x() - person_radius), cur_person->y_speed());
            m_sm->set_next_speed(cur_person, -0.2, cur_person->y_next_speed());
        }
    }
    
    if(move_u_bound < m_u_bound)
    {
        if(m_u_door && move_in_door(move_l_bound, move_r_bound, m_u_door->l_bound(), m_u_door->r_bound()))
        {
            if(!cur_person->pass_door() && cur_person->y_speed() < 0)
            {
                m_sm->set_next_speed(cur_person, 0, -0.5);
                cur_person->pass_door() = true;
            }
        }
        else
        {
            m_sm->set_speed(cur_person, cur_person->x_speed(), -(cur_person->y() - m_u_bound - person_radius));
            m_sm->set_next_speed(cur_person, cur_person->y_next_speed(), 0.2);
        }
    }
    
    if(move_d_bound > m_d_bound)
    {
        if(m_d_door && move_in_door(move_l_bound, move_r_bound, m_d_door->l_bound(), m_d_door->r_bound()))
        {
            if(!cur_person->pass_door() && cur_person->y_speed() > 0)
            {
                m_sm->set_next_speed(cur_person, 0, 0.5);
                cur_person->pass_door() = true;
            }
        }
        else
        {
            m_sm->set_speed(cur_person, cur_person->x_speed(), (m_d_bound - cur_person->y() - person_radius));
            m_sm->set_next_speed(cur_person, cur_person->y_next_speed(), 0.2);
        }
    }
}

bool MapBlock::check_move_to_other_block(Person *cur_person, float person_radius, float move_l_bound, 
                                         float move_r_bound, float move_u_bound, float move_d_bound)
{
    // If there's a neighbor block on r side,
    // and there's a person moving into that block,
    // push the person to that block and delete him in this block
    if(move_r_bound > (m_r_bound+30))
    {
        cur_person->pass_door() = false;
        cur_person->x_speed() = cur_person->x_speed() - (move_r_bound - (m_r_bound+30));
        m_r_neighbor->people().push_back(cur_person);
        m_r_neighbor->people_num() += 1;
        return true;
    }
        
    if(move_l_bound < (m_l_bound-30))
    {
        cur_person->pass_door() = false;
        cur_person->x_speed() = cur_person->x_speed() - (move_l_bound - (m_l_bound-30));
        m_l_neighbor->people().push_back(cur_person);
        m_l_neighbor->people_num() += 1;
        return true;
    }

    if(move_u_bound < (m_u_bound-30))
    {
        cur_person->pass_door() = false;
        cur_person->y_speed() = cur_person->y_speed() - (move_u_bound - (m_u_bound-30));
        m_u_neighbor->people().push_back(cur_person);
        m_u_neighbor->people_num() += 1;
        return true;
    }

    if(move_d_bound > (m_d_bound+30))
    {
        cur_person->pass_door() = false;
        cur_person->y_speed() = cur_person->y_speed() - (move_d_bound - (m_d_bound+30));
        m_d_neighbor->people().push_back(cur_person);
        m_d_neighbor->people_num() += 1;
        return true;
    }

    return false;
}

void MapBlock::delete_people_from_block(std::vector<size_t> &erase_id)
{
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

void MapBlock::check_move_collision()
{
    std::vector<size_t> erase_id;

    for(size_t i=0; i<m_people_num; ++i)
    {
        float move_x_pos = m_people[i]->x() + m_people[i]->x_speed();
        float move_y_pos = m_people[i]->y() + m_people[i]->y_speed();
        float person_radius = m_people[i]->r();
        float move_l_bound = move_x_pos - person_radius;
        float move_r_bound = move_x_pos + person_radius;
        float move_u_bound = move_y_pos - person_radius;
        float move_d_bound = move_y_pos + person_radius;

        check_walls_collision(m_people[i], person_radius, move_l_bound,
                              move_r_bound, move_u_bound, move_d_bound);
        if(check_move_to_other_block(
                        m_people[i], person_radius, move_l_bound,
                        move_r_bound, move_u_bound, move_d_bound))
            erase_id.push_back(i);
    }

    delete_people_from_block(erase_id);
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
                m_sm->reset_speed(m_people[i]);
                m_sm->reset_speed(m_people[num]);
                m_sm->set_stay_away_speed(m_people[i], m_people[num], move_x_pos-cur_move_x_pos, move_y_pos-cur_move_y_pos);

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

void MapBlock::update_person_panic_degree(Person *cur_person, float gap)
{
    if(gap < 10 && cur_person->panic_degree() < 5)
        cur_person->panic_degree() = 5;
    else if(gap >= 20 && gap < 60 && cur_person->panic_degree() < 4)
        cur_person->panic_degree() = 4;
    else if(gap >= 60 && gap < 100 && cur_person->panic_degree() < 3)
        cur_person->panic_degree() = 3;
    else if(gap >= 100 && gap < 140 && cur_person->panic_degree() < 2)
        cur_person->panic_degree() = 2;
    else if(gap >= 140 && gap < 180 && cur_person->panic_degree() < 1)
        cur_person->panic_degree() = 1;
}

void MapBlock::check_fire_collision(std::vector<Fire *>& fire)
{
    std::vector<size_t> erase_id;

    for(size_t j=0; j<fire.size(); ++j)
    {
        size_t x = fire[j]->x();
        size_t y = fire[j]->y();
        size_t r = fire[j]->r();

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
                m_dead_people.push_back(m_people[i]);
                erase_id.push_back(i);
            }
            else
            {
                float gap = distance - collision_bound;
                update_person_panic_degree(m_people[i], gap);
            }
        }
    }

    delete_people_from_block(erase_id);
}

void MapBlock::get_min_door_distance(float x, float y, float &r_x_speed, float &r_y_speed)
{
    float min_door_distance = std::numeric_limits<float>::max();

    // left door
    if(m_l_door)
    {
        float l_door_x = m_l_door->r_bound() - 30;
        float l_door_y = (m_l_door->u_bound() + m_l_door->d_bound()) / 2;
        float l_distance = sqrt((x-l_door_x) * (x-l_door_x) + (y-l_door_y) * (y-l_door_y));
            
        if((m_has_out_door && m_l_door->go_outside() && (l_distance < min_door_distance))
            || (!m_has_out_door && (l_distance < min_door_distance)))
        {
            min_door_distance = l_distance;
            r_x_speed = l_door_x - x;
            r_y_speed = l_door_y - y;
        }      
    }
    
    // right door
    if(m_r_door)
    {
        float r_door_x = m_r_door->l_bound() + 30;
        float r_door_y = (m_r_door->u_bound() + m_r_door->d_bound()) / 2;
        float r_distance = sqrt((x-r_door_x) * (x-r_door_x) + (y-r_door_y) * (y-r_door_y));
        if((m_has_out_door && m_r_door->go_outside() && (r_distance < min_door_distance))
           || (!m_has_out_door && (r_distance < min_door_distance)))
        {
            min_door_distance = r_distance;
            r_x_speed = r_door_x - x;
            r_y_speed = r_door_y - y;
        }      
    }
    
    // up door
    if(m_u_door)
    {
        float u_door_x = (m_u_door->l_bound() + m_u_door->r_bound()) / 2;
        float u_door_y = m_u_door->d_bound() - 30;
        float u_distance = sqrt((x-u_door_x) * (x-u_door_x) + (y-u_door_y) * (y-u_door_y));
        if((m_has_out_door && m_u_door->go_outside() && (u_distance < min_door_distance))
           || (!m_has_out_door && (u_distance < min_door_distance)))
        {
            min_door_distance = u_distance;
            r_x_speed = u_door_x - x;
            r_y_speed = u_door_y - y;
        }      
    }
    
    // down door
    if(m_d_door)
    {
        float d_door_x = (m_d_door->l_bound() + m_d_door->r_bound()) / 2;
        float d_door_y = m_d_door->u_bound() + 30;
        float d_distance = sqrt((x-d_door_x) * (x-d_door_x) + (y-d_door_y) * (y-d_door_y));
        
        if((m_has_out_door && m_d_door->go_outside() && (d_distance < min_door_distance))
           || (!m_has_out_door && (d_distance < min_door_distance)))
        {
            min_door_distance = d_distance;
            r_x_speed = d_door_x - x;
            r_y_speed = d_door_y - y;
        }    
    }
}

void MapBlock::update_people_speed()
{    
    for(size_t i=0; i<m_people_num; ++i)
    {    
        if(m_people[i]->pass_door())
        {
            m_sm->update_speed_in_door(m_people[i]);
        }    
        else 
        {
            float x_speed_to_door, y_speed_to_door;
            get_min_door_distance(m_people[i]->x(), m_people[i]->y(), x_speed_to_door, y_speed_to_door);
            m_sm->update_speed_by_panic_degree(m_people[i], x_speed_to_door, y_speed_to_door);
        }

        m_sm->update_block_speed(m_people[i]);
    }
}

void MapBlock::people_move()
{
    for(size_t i=0; i<m_people_num; ++i)
    {
        m_people[i]->move();
    }
}

void MapBlock::update_map_block(std::vector<Fire *>& fire, uint32_t count)
{   
    if(count > 500)
        check_fire_collision(fire);
        
    check_move_collision();
    check_people_collision();
    people_move();
    update_people_speed();
}