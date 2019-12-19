#include "map/map_block.hpp"
#include "object/fire.hpp"

#include <SDL2/SDL.h>
#include <stdint.h> 
#include <vector>

class WindowMap
{
public:
    WindowMap();
    ~WindowMap();

    void init_walls();   
    void init_map_block();
    void init_fire();
    void delete_map();

    void update_people(); 
    void render_people();
    void check_fire_collision();
    void update_fire();
    void render_fire();
    bool update_screen();

    bool check_all_alive_indoor();

    size_t get_alive_num();

private:
    SDL_Renderer* gRenderer;
    SDL_Window* gWindow;
    std::vector<SDL_Rect> m_walls;
    MapBlock *lu_block;
    MapBlock *ld_block;
    MapBlock *ru_block;
    MapBlock *rd_block;
    MapBlock *outside_block;

    std::vector<Fire *> m_fire;
    uint32_t count;

    const size_t SCREEN_WIDTH = 590;
    const size_t SCREEN_HEIGHT = 590;
};
