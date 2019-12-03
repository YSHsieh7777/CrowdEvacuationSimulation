#include "simulator/window_map.hpp"

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
		gWindow = SDL_CreateWindow( "Evacuation Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	if(!success)
		exit(1);
}

WindowMap::~WindowMap()
{
	delete_people();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void WindowMap::init_walls()
{
	m_walls = std::vector<SDL_Rect>(12);

    m_walls[0].x = 50;
    m_walls[0].y = 50;
    m_walls[0].w = 490;
    m_walls[0].h = 30;

    m_walls[1].x = 50;
    m_walls[1].y = 280;
    m_walls[1].w = 120;
    m_walls[1].h = 30;

    m_walls[2].x = 190;
    m_walls[2].y = 280;
    m_walls[2].w = 210;
    m_walls[2].h = 30;

    m_walls[3].x = 420;
    m_walls[3].y = 280;
    m_walls[3].w = 120;
    m_walls[3].h = 30;

    m_walls[4].x = 50;
    m_walls[4].y = 510;
    m_walls[4].w = 350;
    m_walls[4].h = 30;

    m_walls[5].x = 420;
    m_walls[5].y = 510;
    m_walls[5].w = 120;
    m_walls[5].h = 30;

    m_walls[6].x = 50;
    m_walls[6].y = 50;
    m_walls[6].w = 30;
    m_walls[6].h = 120;

    m_walls[7].x = 50;
    m_walls[7].y = 190;
    m_walls[7].w = 30;
    m_walls[7].h = 350;

    m_walls[8].x = 280;
    m_walls[8].y = 50;
    m_walls[8].w = 30;
    m_walls[8].h = 120;

    m_walls[9].x = 280;
    m_walls[9].y = 190;
    m_walls[9].w = 30;
    m_walls[9].h = 210;

    m_walls[10].x = 280;
    m_walls[10].y = 420;
    m_walls[10].w = 30;
    m_walls[10].h = 120;

    m_walls[11].x = 510;
    m_walls[11].y = 50;
    m_walls[11].w = 30;
    m_walls[11].h = 490;
}

const std::vector<SDL_Rect> & WindowMap::walls() const
{
    return m_walls;
}

void WindowMap::init_people()
{
	lu_people = new MapBlock(80, 280, 80, 280, 7, 7, true, true, false, true);
	ld_people = new MapBlock(80, 280, 310, 510, 7, 7, false, true, true, false);
	ru_people = new MapBlock(310, 510, 80, 280, 7, 7, true, false, false, true);
	rd_people = new MapBlock(310, 510, 310, 510, 7, 7, true, false, true, true);
	lu_people->add_neighbors(NULL, ru_people, NULL, ld_people);
	ld_people->add_neighbors(NULL, rd_people, lu_people, NULL);
	ru_people->add_neighbors(lu_people, NULL, NULL, rd_people);
	rd_people->add_neighbors(ld_people, NULL, ru_people, NULL);
}

void WindowMap::delete_people()
{
	delete lu_people;
	delete ld_people;
	delete ru_people;
	delete rd_people;
}

void WindowMap::update_people()
{
	lu_people->update_people();
	ru_people->update_people();
	ld_people->update_people();
	rd_people->update_people();
}

void WindowMap::rander_people()
{
	size_t lu_size = lu_people->people_num();
	size_t ru_size = ru_people->people_num();
	size_t ld_size = ld_people->people_num();
	size_t rd_size = rd_people->people_num();

	for(size_t i=0; i<lu_size; ++i)
		lu_people->people()[i].render(gRenderer);
	for(size_t i=0; i<ru_size; ++i)
		ru_people->people()[i].render(gRenderer);
	for(size_t i=0; i<ld_size; ++i)
		ld_people->people()[i].render(gRenderer);
	for(size_t i=0; i<rd_size; ++i)
		rd_people->people()[i].render(gRenderer);
}

void WindowMap::update_screen()
{
	//Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );
	
	//Render wall
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
	for(int i=0; i<12; i++)
	{
		SDL_RenderDrawRect( gRenderer, &m_walls[i] );
	}						

	update_people();
	//Render person
	rander_people();

	//Update screen
	SDL_RenderPresent( gRenderer );
}