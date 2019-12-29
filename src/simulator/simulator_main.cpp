#include "map/window_map.hpp"

#include <iostream>
#include <time.h>

int main( int argc, char* args[] )
{
    WindowMap *window_map = new WindowMap();

    // Create walls and people on the map
    window_map->init_walls();
    window_map->init_map_block();
    window_map->init_fire();

    //Event handler
    SDL_Event e;
    bool is_alive = true;
    
    // record the begin time
    clock_t begin = clock();
    double total_frame = 0;

    while(1)
    {
        total_frame++;

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0)
        {
            //User requests quit
            if(e.type == SDL_QUIT)
            {
                exit(0);
            }
        }

        if(!(is_alive = window_map->update_screen()))
            break;  
    }

    // record the end time and caculate the average fps
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    double fps = total_frame / time_spent;
    std::cout << "average fps: " << fps << '\n';

    std::cout << "alive people number: " << window_map->get_alive_num() << '\n';
    delete window_map;

    return 0;
}