#include "simulator/simulator_main.hpp"

#include <iostream>
#include <time.h>

void init_map(WindowMap *window_map)
{
    // Create walls and people on the map
    window_map->init_walls();
    window_map->init_map_block();
    window_map->init_fire();
}

void handle_event(SDL_Event &e)
{
    //Handle events on queue
    while(SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if(e.type == SDL_QUIT)
        {
            exit(0);
        }
    }
}

void update_map(WindowMap *window_map)
{
    window_map->clear_screen();
    window_map->update_screen();
}

int main(int argc, char* args[])
{
    WindowMap *window_map = new WindowMap();
    init_map(window_map);

    //Event handler
    SDL_Event e;
    
    // record the begin time
    clock_t begin = clock();
    double total_frame = 0;

    while(1)
    {
        total_frame++;
        handle_event(e);
        update_map(window_map);
        if(!window_map->check_all_alive_indoor())
            break;
    }

    // record the end time and calculate the average fps
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    double fps = total_frame / time_spent;
    std::cout << "average fps: " << fps << '\n';

    std::cout << "alive people number: " << window_map->get_alive_num() << '\n';
    delete window_map;

    return 0;
}