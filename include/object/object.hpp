#include <SDL2/SDL.h>
#include <vector>
#include <stdint.h>

#pragma once

class Object
{
public:
    Object() = default;
    ~Object() = default;

    void setColor(SDL_Renderer* gRenderer, uint8_t r, uint8_t g, uint8_t b)
    {
        SDL_SetRenderDrawColor(gRenderer, r, g, b, 0xFF);
    }
    
    virtual void render(SDL_Renderer* ) = 0;
};