#include "map/window_map.hpp"

#include "gtest/gtest.h"

TEST(MapBlockTest, PeopleNumber) {
    MapBlock *block = new MapBlock(0, 0, 0, 0, 5, 5, NULL, NULL, NULL, NULL);

    EXPECT_EQ(block->people_num(), block->people().size());
    EXPECT_EQ(block->people_num(), 25);

    delete block;
}

TEST(MapBlockTest, WallCollisionTest) {
    MapBlock *block = new MapBlock(0, 200, 0, 200, 1, 1, NULL, NULL, NULL, NULL);

    block->people()[0]->x_speed() = 200;
    block->check_walls_collision();
    block->check_people_collision();
    block->people_move();

    EXPECT_LE(block->people()[0]->x(), 200);

    delete block;
}

TEST(MapBlockTest, PeopleCollisionTest) {
    MapBlock *block = new MapBlock(0, 200, 0, 200, 1, 2, NULL, NULL, NULL, NULL);

    block->people()[0]->x() = 50;
    block->people()[0]->y() = 50;
    block->people()[1]->x() = 66;
    block->people()[1]->y() = 50;
    block->people()[0]->x_speed() = 1;
    block->people()[1]->x_speed() = -1;
    block->check_walls_collision();
    block->check_people_collision();
    block->people_move();

    EXPECT_EQ(block->people()[0]->x(), 50);
    EXPECT_EQ(block->people()[1]->x(), 66);

    delete block;
}

TEST(MapBlockTest, FireCollisionTest) {
    MapBlock *block = new MapBlock(0, 200, 0, 200, 1, 1, NULL, NULL, NULL, NULL);

    block->people()[0]->x() = 10;
    block->people()[0]->y() = 10;
    block->check_fire_collision(0, 0, 11);

    EXPECT_EQ(block->people()[0]->is_dead(), true);

    delete block;
}

int main(int argc, char** argv)  
{
    int iret;
    testing::InitGoogleTest(&argc, argv); 
    iret = RUN_ALL_TESTS(); 
    return iret;
}