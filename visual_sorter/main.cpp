#include "SDL.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

//draw color
#define DRAW_COLOR_R 0xFF
#define DRAW_COLOR_G 0xFF
#define DRAW_COLOR_B 0xFF
#define DRAW_COLOR_A SDL_ALPHA_OPAQUE

//clear color
#define CLEAR_COLOR_R 0x00
#define CLEAR_COLOR_G 0x00
#define CLEAR_COLOR_B 0x00
#define CLEAR_COLOR_A SDL_ALPHA_OPAQUE

struct layout {
    int numTiles;
    int rows;
    int cols;
};

struct visualSorter {
    SDL_Window *mainWin;
    SDL_Renderer *mainRenderer;
    struct layout layout;
};

bool init(struct visualSorter *sorter) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
	SDL_Log("Error SDL_Init: %s", SDL_GetError());
	return false;
    }

    //Set texture filtering to linear
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
    	printf( "Warning: Linear texture filtering not enabled!" );
    }
    
    //Create window
    sorter->mainWin = SDL_CreateWindow( "Visual Sorter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( sorter->mainWin == NULL )
    {
    	printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    	return false;
    }
    else
    {
    	//Create renderer for window
    	sorter->mainRenderer = SDL_CreateRenderer( sorter->mainWin, -1, SDL_RENDERER_ACCELERATED );
    
    	//try software render if hardware fails
    	if( sorter->mainRenderer == NULL )
    	{
    		SDL_Log( "Accelerated renderer could not be created! SDL Error: %s\nSwitching to software renderer", SDL_GetError() );
    		sorter->mainRenderer = SDL_CreateRenderer( sorter->mainWin, -1, SDL_RENDERER_SOFTWARE);
    	}
    
    	if( sorter->mainRenderer == NULL )
    	{
    		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
    		return false;
    	}
    }
    return true;
}

void close(struct visualSorter *sorter) {

    //Destroy window	
    SDL_DestroyRenderer(sorter->mainRenderer);
    SDL_DestroyWindow(sorter->mainWin);
    sorter->mainWin = NULL;
    sorter->mainRenderer = NULL;

    SDL_Quit();

}

bool clearScreen(struct visualSorter sorter) {
    SDL_SetRenderDrawColor(sorter.mainRenderer, CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A);
    if(0 > SDL_RenderClear(sorter.mainRenderer)) {
	SDL_Log("SDL_RenderClear: error = %s", SDL_GetError());
	return false;
    }

	return true;
}

//draw the borders around sort windows
bool drawBorders(struct visualSorter sorter) {
    SDL_Rect border = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 10, SCREEN_HEIGHT/3};
    SDL_SetRenderDrawColor(sorter.mainRenderer, DRAW_COLOR_R, DRAW_COLOR_G, DRAW_COLOR_B, DRAW_COLOR_A);
    if(0 > SDL_RenderFillRect(sorter.mainRenderer, &border)) {
	SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
	return false;
    }

    return true;
}

bool drawScreen(struct visualSorter sorter) {
    if(!clearScreen(sorter)) {
	return false;
    }

    if(!drawBorders(sorter)) {
	return false;
    }

    return true;
}

bool loopHandler(struct visualSorter sorter) {
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
	//user requests to quit
	if(e.type == SDL_QUIT) {
	    return true;
	}
    }

    if(!drawScreen(sorter)) {
	return true;
    }

    //Update Screen
    SDL_RenderPresent(sorter.mainRenderer);

    return false;

}

void getLayout(struct layout *layout) {
    layout->numTiles = 2;
    layout->rows = 1;
    layout->cols = 2;
}

int main(int argc, char* argv[]) {
    struct visualSorter sorter = {NULL, NULL, {0,0,0}};
    bool quit = false;

    //intialized SDL, create window
    if(!init(&sorter)) {
	SDL_Log("Error initializing SDL");
	quit = true;
    }

    getLayout(&sorter.layout);

    while(!quit) {
	quit = loopHandler(sorter);
    }

    close(&sorter);
}

