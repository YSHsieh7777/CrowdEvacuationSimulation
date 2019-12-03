//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

#include "map_block.hpp"

class WindowMap
{
public:
    WindowMap();
    ~WindowMap();

    void init_walls();   
    const std::vector<SDL_Rect> & walls() const;

    void init_people();
    void delete_people();
    void update_people();
    void update_screen();
    void rander_people();

private:
    SDL_Renderer* gRenderer;
    SDL_Window* gWindow;
	std::vector<SDL_Rect> m_walls;
	MapBlock *lu_people;
    MapBlock *ld_people;
    MapBlock *ru_people;
    MapBlock *rd_people;

    const size_t SCREEN_WIDTH = 590;
    const size_t SCREEN_HEIGHT = 590;
};
