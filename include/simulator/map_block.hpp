//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <math.h>
#include <iostream>

#include "person.hpp"

class MapBlock
{
public:
    MapBlock(size_t , size_t , size_t , size_t , size_t , size_t , bool , bool , bool , bool );
    ~MapBlock();

    const size_t & people_num() const;
    size_t & people_num();
    const std::vector<Person* > & people() const;
    std::vector<Person* > & people();

    void init_people();

    void update_people_speed();
    void check_walls_collision();
    void check_person_collision(size_t );
    void check_people_collision();
    void update_people();

    void add_neighbors(MapBlock* , MapBlock* , MapBlock* , MapBlock* );

private:
	std::vector<Person *> m_people;

    size_t m_left_bound;
    size_t m_right_bound;
    size_t m_up_bound;
    size_t m_bottom_bound;
    size_t m_people_num;

    bool m_left_door;
    bool m_right_door;
    bool m_up_door;
    bool m_bottom_door;

    MapBlock *m_l_neighbor = NULL;
    MapBlock *m_r_neighbor = NULL;
    MapBlock *m_u_neighbor = NULL;
    MapBlock *m_d_neighbor = NULL;

    float m_average_x, m_average_y;
};