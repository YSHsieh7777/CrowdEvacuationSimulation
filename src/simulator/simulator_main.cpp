#include "map/window_map.hpp"

#include <iostream>

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

        is_alive = window_map->update_screen();
        if(!is_alive)
            break;
    }

    std::cout << "alive people number: " << window_map->get_alive_num() << '\n';
    delete window_map;

    return 0;
}