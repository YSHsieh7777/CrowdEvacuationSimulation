#include "map/window_map.hpp"

WindowMap::WindowMap()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        m_gWindow = SDL_CreateWindow( "Evacuation Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( m_gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            m_gRenderer = SDL_CreateRenderer( m_gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( m_gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( m_gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            }
        }
    }

    // If there's an error when initializing the SDL, exit
    if(!success)
        exit(1);
}

WindowMap::~WindowMap()
{
    delete_map();

    //Destroy window    
    SDL_DestroyRenderer( m_gRenderer );
    SDL_DestroyWindow( m_gWindow );
    m_gWindow = NULL;
    m_gRenderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void WindowMap::init_walls()
{
    m_walls = std::vector<SDL_Rect>(12);

    // Draw the walls in a brute way
    m_walls[0].x = 50;
    m_walls[0].y = 50;
    m_walls[0].w = 490;
    m_walls[0].h = 30;

    m_walls[1].x = 50;
    m_walls[1].y = 280;
    m_walls[1].w = 110;
    m_walls[1].h = 30;

    m_walls[2].x = 200;
    m_walls[2].y = 280;
    m_walls[2].w = 190;
    m_walls[2].h = 30;

    m_walls[3].x = 430;
    m_walls[3].y = 280;
    m_walls[3].w = 110;
    m_walls[3].h = 30;

    m_walls[4].x = 50;
    m_walls[4].y = 510;
    m_walls[4].w = 340;
    m_walls[4].h = 30;

    m_walls[5].x = 430;
    m_walls[5].y = 510;
    m_walls[5].w = 110;
    m_walls[5].h = 30;

    m_walls[6].x = 50;
    m_walls[6].y = 50;
    m_walls[6].w = 30;
    m_walls[6].h = 110;

    m_walls[7].x = 50;
    m_walls[7].y = 200;
    m_walls[7].w = 30;
    m_walls[7].h = 340;

    m_walls[8].x = 280;
    m_walls[8].y = 50;
    m_walls[8].w = 30;
    m_walls[8].h = 110;

    m_walls[9].x = 280;
    m_walls[9].y = 200;
    m_walls[9].w = 30;
    m_walls[9].h = 190;

    m_walls[10].x = 280;
    m_walls[10].y = 430;
    m_walls[10].w = 30;
    m_walls[10].h = 110;

    m_walls[11].x = 510;
    m_walls[11].y = 50;
    m_walls[11].w = 30;
    m_walls[11].h = 490;
}

void WindowMap::init_map_block()
{
    Door *l_door = new Door(50, 80, 160, 200, true);
    Door *r_door = new Door(280, 310, 160, 200, false);
    Door *u_door = NULL;
    Door *d_door = new Door(160, 200, 280, 310, false);
    lu_block = new MapBlock(80, 280, 80, 280, 3, 3, l_door, r_door, u_door, d_door, true);

    l_door = NULL;
    r_door = new Door(280, 310, 390, 430, false);
    u_door = new Door(160, 200, 280, 310, false);
    d_door = NULL;
    ld_block = new MapBlock(80, 280, 310, 510, 6, 6, l_door, r_door, u_door, d_door, false);

    l_door = new Door(280, 310, 160, 200, false);
    r_door = NULL;
    u_door = NULL;
    d_door = new Door(390, 430, 280, 310, false);
    ru_block = new MapBlock(310, 510, 80, 280, 6, 6, l_door, r_door, u_door, d_door, false);

    l_door = new Door(280, 310, 390, 430, false);
    r_door = NULL;
    u_door = new Door(390, 430, 280, 310, false);
    d_door = new Door(390, 430, 510, 540, true);
    rd_block = new MapBlock(310, 510, 310, 510, 3, 3, l_door, r_door, u_door, d_door, true);

    outside_block = new MapBlock(0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, false);

    lu_block->add_neighbors(outside_block, ru_block, outside_block, ld_block);
    ld_block->add_neighbors(outside_block, rd_block, lu_block, outside_block);
    ru_block->add_neighbors(lu_block, outside_block, outside_block, rd_block);
    rd_block->add_neighbors(ld_block, outside_block, ru_block, outside_block);
}

void WindowMap::init_fire()
{
    m_count = 0;

    Fire *f1 = new Fire(80, 510, 0);
    m_fire.push_back(f1);
    Fire *f2 = new Fire(510, 80, 0);
    m_fire.push_back(f2);
}

void WindowMap::delete_map()
{
    delete lu_block;
    delete ld_block;
    delete ru_block;
    delete rd_block;
    delete outside_block;
    delete m_fire[0];
    delete m_fire[1];
}

void WindowMap::update_blocks()
{
    // People move in the four blocks
    lu_block->update_map_block(m_fire, m_count);
    ru_block->update_map_block(m_fire, m_count);
    ld_block->update_map_block(m_fire, m_count);
    rd_block->update_map_block(m_fire, m_count);
}

void WindowMap::render_people()
{
    // Render each people in the four blocks
    for(size_t i=0; i<lu_block->people_num(); ++i)
        lu_block->people()[i]->render(m_gRenderer);
    for(size_t i=0; i<ru_block->people_num(); ++i)
        ru_block->people()[i]->render(m_gRenderer);
    for(size_t i=0; i<ld_block->people_num(); ++i)
        ld_block->people()[i]->render(m_gRenderer);
    for(size_t i=0; i<rd_block->people_num(); ++i)
        rd_block->people()[i]->render(m_gRenderer);
}


void WindowMap::update_fire()
{
    // Update fire and check if the people is killed by fire
    if(m_count > 500)
    {
        for(size_t i=0; i<2; ++i)
        {
            m_fire[i]->update_radius();
        }
    }
    else
    {
        m_count++;
    }  
}

void WindowMap::render_fire()
{
    for(size_t i=0; i<2; ++i)
    {
        m_fire[i]->render(m_gRenderer);
    }
}

void WindowMap::clear_screen()
{
    // Clear screen
    SDL_SetRenderDrawColor( m_gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( m_gRenderer );
}

void WindowMap::render_walls()
{
    // Render wall
    SDL_SetRenderDrawColor( m_gRenderer, 0x00, 0x00, 0x00, 0xFF );
    for(int i=0; i<12; i++)
    {
        SDL_RenderDrawRect( m_gRenderer, &m_walls[i] );
    }    
}

bool WindowMap::update_screen()
{ 
    update_fire();
    update_blocks();

    render_walls();
    render_fire();
    render_people();
    
    // Update screen
    SDL_RenderPresent( m_gRenderer );

    return true;
}

bool WindowMap::check_all_alive_indoor()
{
    if((lu_block->people_num() == 0) && (ld_block->people_num() == 0) && 
        (ru_block->people_num() == 0) && (rd_block->people_num() == 0))
        return false;

    return true;
}

size_t WindowMap::get_alive_num()
{
    return outside_block->people_num();
}
