//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <limits>

#include "person.hpp"
#include "door.hpp"

class MapBlock
{
public:
    MapBlock(size_t , size_t , size_t , size_t , size_t , size_t , Door* , Door* , Door* , Door* );
    ~MapBlock();

    const size_t & people_num() const;
    size_t & people_num();
    const std::vector<Person* > & people() const;
    std::vector<Person* > & people();

    void init_people();

    void get_min_door_distance(float , float , float & , float & );
    void update_people_speed();
    void check_move_to_other_block();
    void check_walls_collision();
    void check_person_collision(size_t );
    void check_people_collision();
    void check_fire_collision(size_t , size_t , size_t , size_t );
    void people_move();
    void update_fire();
    void update_map_block();

    void add_neighbors(MapBlock* , MapBlock* , MapBlock* , MapBlock* );

private:
    std::vector<Person *> m_people;
    std::vector<Person *> m_dead_people;

    size_t m_l_bound;
    size_t m_r_bound;
    size_t m_u_bound;
    size_t m_d_bound;

    size_t m_people_num;

    Door* m_l_door;
    Door* m_r_door;
    Door* m_u_door;
    Door* m_d_door;

    MapBlock *m_l_neighbor = NULL;
    MapBlock *m_r_neighbor = NULL;
    MapBlock *m_u_neighbor = NULL;
    MapBlock *m_d_neighbor = NULL;

    float m_average_x, m_average_y;
};