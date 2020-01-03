#include "map/map_block.hpp"
#include "object/wall.hpp"

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

    void update_blocks(); 
    void render_people();
    void update_fire();
    void render_fire();
    void render_walls();
    void clear_screen();
    bool update_screen();

    bool check_all_alive_indoor();

    size_t get_alive_num();

private:
    SDL_Renderer* m_gRenderer;
    SDL_Window* m_gWindow;

    MapBlock *lu_block;
    MapBlock *ld_block;
    MapBlock *ru_block;
    MapBlock *rd_block;
    MapBlock *outside_block;
    Wall *m_walls;

    std::vector<Fire *> m_fire;
    uint32_t m_count;

    const size_t SCREEN_WIDTH = 590;
    const size_t SCREEN_HEIGHT = 590;
};
