#include "object/object.hpp"

#include <SDL2/SDL.h>
#include <vector>

class Wall final : public Object
{
public:
    Wall();
    ~Wall() = default;

    //Shows the walls on the screen
    void render(SDL_Renderer* ) override;

private:
    std::vector<SDL_Rect> m_walls;
};