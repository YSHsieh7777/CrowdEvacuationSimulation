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
	for(uint8_t y=1; y<7; ++y)
	{
		for(uint8_t x=1; x<7; ++x)
		{	
			float rand_x = x*25 + (17.5 - 7.5) * rand() / (RAND_MAX + 1.0) + 7.5;
			float rand_y = y*25 + (17.5 - 7.5) * rand() / (RAND_MAX + 1.0) + 7.5;
			
			Person lu_p(80 + rand_x, 80 + rand_y, 7.5);
			lu_people.push_back(lu_p);
			Person ru_p(310 + rand_x, 80 + rand_y, 7.5);
			ru_people.push_back(ru_p);
			Person ld_p(80 + rand_x, 310 + rand_y, 7.5);
			ld_people.push_back(ld_p);
			Person rd_p(310 + rand_x, 310 + rand_y, 7.5);
			rd_people.push_back(rd_p);
		}
	}		
}

void WindowMap::update_lu_people_speed()
{
	size_t lu_size = lu_people.size();
	
	for(size_t i=0; i<lu_size; ++i)
	{	
		if(lu_people[i].pass_door())
		{
			lu_people[i].x_speed() = lu_people[i].x_next_speed();
			lu_people[i].y_speed() = lu_people[i].y_next_speed();
		}	
		else 
		{
			lu_people[i].x_speed() = lu_people[i].x_next_speed() + rand() / (RAND_MAX + 1.0) - 0.5;
			lu_people[i].y_speed() = lu_people[i].y_next_speed() + rand() / (RAND_MAX + 1.0) - 0.5;
		}	
	}
}

void WindowMap::check_lu_walls_collision()
{
	size_t lu_size = lu_people.size();
	
	for(size_t i=0; i<lu_size; ++i)
	{
		float move_x_pos = lu_people[i].x() + lu_people[i].x_speed();
		float move_y_pos = lu_people[i].y() + lu_people[i].y_speed();

		if((move_x_pos-7.5) < 80 && ((move_y_pos-7.5) < 170 || (move_y_pos+7.5) > 190))
		{
			lu_people[i].x_speed() =  -(lu_people[i].x() - 80 - 7.5);
			lu_people[i].x_next_speed() = 0;
		}
		else if((move_x_pos-7.5) < 80 && ((move_y_pos-7.5) >= 170 || (move_y_pos+7.5) <= 190))
		{
			lu_people[i].y_next_speed() = 0;
			lu_people[i].pass_door() = true;
		}	
		else if((move_x_pos+7.5) > 280 && ((move_y_pos-7.5) < 170 || (move_y_pos+7.5) > 190))
		{
			lu_people[i].x_speed() =  280 - lu_people[i].x() - 7.5;
			lu_people[i].x_next_speed() = 0;
		}
		else if((move_x_pos+7.5) > 280 && ((move_y_pos-7.5) >= 170 && (move_y_pos+7.5) <= 190))
		{
			lu_people[i].y_next_speed() = 0;
			lu_people[i].pass_door() = true;
		}
			
		if((move_y_pos-7.5) < 80)
		{
			lu_people[i].y_speed() =  -(lu_people[i].y() - 80 - 7.5);
			lu_people[i].y_next_speed() = 0;
		}	
		else if((move_y_pos+7.5) > 280 && ((move_x_pos-7.5) < 170 || (move_x_pos+7.5) > 190))
		{
			lu_people[i].y_speed() =  280 - lu_people[i].y() - 7.5;
			lu_people[i].y_next_speed() = 0;
		}
		else if((move_y_pos+7.5) > 280 && ((move_x_pos-7.5) >= 170 || (move_x_pos+7.5) <= 190))
		{
			lu_people[i].x_speed() = 0;
			lu_people[i].pass_door() = true;
		}

		if((move_x_pos-7.5) > 310)
		{
			ru_people.push_back(lu_people[i]);
			lu_people.erase(lu_people.begin()+i);
		}
		else if((move_y_pos-7.5) > 310)
		{
			ld_people.push_back(lu_people[i]);
			lu_people.erase(lu_people.begin()+i);
		}
	}
}

void WindowMap::check_lu_person_collision(size_t num)
{
	float cur_move_x_pos = lu_people[num].x() + lu_people[num].x_speed();
	float cur_move_y_pos = lu_people[num].y() + lu_people[num].y_speed();

	size_t lu_size = lu_people.size();

	for(size_t i=0; i<lu_size; ++i)
	{
		float move_x_pos = lu_people[i].x() + lu_people[i].x_speed();
		float move_y_pos = lu_people[i].y() + lu_people[i].y_speed();

		if(i == num)
			continue;
		else
		{
			if((abs(cur_move_x_pos-move_x_pos) < 15) && (abs(cur_move_y_pos-move_y_pos) < 15))
			{
				lu_people[num].x_speed() = 0;
				lu_people[num].y_speed() = 0;
				lu_people[i].x_speed() = 0;
				lu_people[i].y_speed() = 0;
				lu_people[num].x_next_speed() = lu_people[num].x_next_speed() + (cur_move_x_pos-move_x_pos) * 0.01;
				lu_people[num].y_next_speed() = lu_people[num].y_next_speed() + (cur_move_y_pos-move_y_pos) * 0.01;
				lu_people[i].x_next_speed() = lu_people[i].x_next_speed() + (move_x_pos-cur_move_x_pos) * 0.01;
				lu_people[i].y_next_speed() = lu_people[i].y_next_speed() + (move_y_pos-cur_move_y_pos) * 0.01;
				check_lu_person_collision(i);
				check_lu_person_collision(num);
				break;
			}
		}	
	}
}

void WindowMap::check_lu_people_collision()
{
	size_t lu_size = lu_people.size();
	
	for(size_t i=0; i<lu_size; ++i)
	{
		check_lu_person_collision(i);
	}
}

void WindowMap::update_lu_people()
{
	check_lu_walls_collision();
	check_lu_people_collision();
	
	size_t lu_size = lu_people.size();
	
	for(size_t i=0; i<lu_size; ++i)
	{
		lu_people[i].move();
	}

	// for(size_t i=0; i<lu_size; ++i)
	// {
	// 	std::cout << "person " << i << "  x: " << lu_people[i].x() << "  y: " << lu_people[i].y() << "\n";
	// }
	
	update_lu_people_speed();
}

void WindowMap::update_people()
{
	update_lu_people();
}

void WindowMap::rander_people()
{
	size_t lu_size = lu_people.size();
	size_t ru_size = ru_people.size();
	size_t ld_size = ld_people.size();
	size_t rd_size = rd_people.size();

	for(size_t i=0; i<lu_size; ++i)
		lu_people[i].render(gRenderer);
	for(size_t i=0; i<ru_size; ++i)
		ru_people[i].render(gRenderer);
	for(size_t i=0; i<ld_size; ++i)
		ld_people[i].render(gRenderer);
	for(size_t i=0; i<rd_size; ++i)
		rd_people[i].render(gRenderer);
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