#include "object/door.hpp"
#include "object/fire.hpp"
#include "utils/speed_manager.hpp"

#include <SDL2/SDL.h>
#include <vector>

#define cacl_two_dim_distance(a_x, a_y, b_x, b_y)  \
    sqrt((a_x-b_x) * (a_x-b_x) + (a_y-b_y) * (a_y-b_y))

#define cacl_one_dim_distance(a, b) \
    a - b

#define add_person_in_block(block, person) \
    block->people().push_back(person); \
    block->people_num() += 1;

#define delete_person(people, num) \
    people.pop_back(); \
    num -= 1;

#define cacl_pos(pos, speed) \
    pos + speed
    

class MapBlock
{
public:
    MapBlock(size_t , size_t , size_t , size_t , size_t , size_t , Door* , Door* , Door* , Door* , bool );
    ~MapBlock();

    const size_t & people_num() const;
    size_t & people_num();
    const std::vector<Person* > & people() const;
    std::vector<Person* > & people();

    void add_neighbors(MapBlock* , MapBlock* , MapBlock* , MapBlock* );

    bool move_in_door(float , float , float , float );
    void delete_people_from_block(std::vector<size_t> & );
    void check_move_collision();
    void check_walls_collision(Person * , float , float , float , float , float );
    bool check_move_to_other_block(Person * , float , float , float , float , float );
    void check_person_collision(size_t );
    void check_people_collision();
    void update_person_panic_degree(Person * , float );
    void check_fire_collision(std::vector<Fire *>& );

    void get_min_door_distance(float , float , float & , float & );
    void update_people_speed();
    void people_move();

    void update_map_block(std::vector<Fire *>& , u_int32_t );

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
    bool m_has_out_door;

    MapBlock *m_l_neighbor = NULL;
    MapBlock *m_r_neighbor = NULL;
    MapBlock *m_u_neighbor = NULL;
    MapBlock *m_d_neighbor = NULL;

    SpeedManager *m_sm = NULL;
};