#include "main.hpp"

int main( int argc, char* args[] )
{
	WindowMap window_map;
	window_map.init_walls();
	window_map.init_people();

	//Event handler
	SDL_Event e;
	
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

		window_map.update_screen();
	}

	return 0;
}