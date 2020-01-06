#include "map/window_map.hpp"

#include "gtest/gtest.h"

TEST(MapBlockTest, PeopleNumber) {
    MapBlock *block = new MapBlock(0, 0, 0, 0, 5, 5, NULL, NULL, NULL, NULL, false);

    EXPECT_EQ(block->alive_people().size(), 25);

    delete block;
}

TEST(CollisionTest, WallCollisionTest) {
    MapBlock *block = new MapBlock(0, 200, 0, 200, 1, 1, NULL, NULL, NULL, NULL, false);

    Person *cur_person = block->alive_people()[0];
    cur_person->x_speed() = 200;
    float move_x_pos = cacl_pos(cur_person->x(), cur_person->x_speed());
    float move_y_pos = cacl_pos(cur_person->y(), cur_person->y_speed());
    float person_radius = cur_person->r();
    float move_l_bound = cacl_one_dim_distance(move_x_pos, person_radius);
    float move_r_bound = cacl_one_dim_distance(move_x_pos, (-person_radius));
    float move_u_bound = cacl_one_dim_distance(move_y_pos, person_radius);
    float move_d_bound = cacl_one_dim_distance(move_y_pos, (-person_radius));

    block->check_walls_collision(cur_person, person_radius, move_l_bound,
                            move_r_bound, move_u_bound, move_d_bound);
    block->people_move();

    EXPECT_LE(block->alive_people()[0]->x(), 200);

    delete block;
}

TEST(CollisionTest, PeopleCollisionTest) {
    MapBlock *block = new MapBlock(0, 200, 0, 200, 1, 2, NULL, NULL, NULL, NULL, false);

    block->alive_people()[0]->x() = 50;
    block->alive_people()[0]->y() = 50;
    block->alive_people()[1]->x() = 66;
    block->alive_people()[1]->y() = 50;
    block->alive_people()[0]->x_speed() = 1;
    block->alive_people()[1]->x_speed() = -1;
    block->check_people_collision();
    block->people_move();

    EXPECT_EQ(block->alive_people()[0]->x(), 50);
    EXPECT_EQ(block->alive_people()[1]->x(), 66);

    delete block;
}

TEST(CollisionTest, FireCollisionTest) {
    MapBlock *block = new MapBlock(0, 200, 0, 200, 1, 1, NULL, NULL, NULL, NULL, false);
    std::vector<Fire *> fire;
    Fire *f = new Fire(0, 0, 11);
    fire.push_back(f);

    block->alive_people()[0]->x() = 10;
    block->alive_people()[0]->y() = 10;
    block->check_fire_collision(fire);

    EXPECT_EQ(block->alive_people().empty(), true);

    delete block;
    delete fire[0];
}

int main(int argc, char** argv)  
{
    int iret;
    testing::InitGoogleTest(&argc, argv); 
    iret = RUN_ALL_TESTS(); 
    return iret;
}