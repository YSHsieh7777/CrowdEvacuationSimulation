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
    
    while(1)
    {
        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0)
        {
            //User requests quit
            if(e.type == SDL_QUIT)
            {
                exit(0);
            }
        }

        // calculate execution time
        clock_t begin = clock();
        if(!(is_alive = window_map->update_screen()))
            break;
        clock_t end = clock();

        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        double fps = 1 / time_spent;

        std::cout << "fps: " << fps << '\n';
    }

    std::cout << "alive people number: " << window_map->get_alive_num() << '\n';
    delete window_map;

    return 0;
}