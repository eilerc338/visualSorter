#include "SDL.h"
#include <stdbool.h>
#include "sort_algorithms.h"

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

#define CUSHION	2	//pixels between bars
#define ARRAY_SIZE 120
#define VAL_MAX ARRAY_SIZE


struct algDesc {
    char friendlyName[FILENAME_MAX];
    bool (*add)(struct entryPoints *ep);
};

struct algDesc list[] = {
    {"Insertion Sort", &insertionAdd},
    {"", NULL}
};

struct algContext {
    struct entryPoints ep;
    void *privateData;
};

/*
struct algData {
    int data[ARRAY_SIZE];
    int size;
};
*/

enum initialState {
    SORTED,
    REVERSE_SORTED,
    RANDOM,
};

struct layout {
    int numTiles;
    int rows;
    int cols;
};

struct bar {
    int heightRes;
    int width;
    int stride;
    int frontPorch;
};

struct visualSorter {
    SDL_Window *mainWin;
    SDL_Renderer *mainRenderer;
    struct layout layout;
    struct bar bar;
    int numAlgs;

    //pointer to a malloced array
    struct algContext *algorithms;
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

void cleanup(struct visualSorter *sorter) {

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
    //this doesn't actually draw a border yet, jut one rectangle
    /*
    SDL_Rect border = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 10, SCREEN_HEIGHT/3};
    SDL_SetRenderDrawColor(sorter.mainRenderer, DRAW_COLOR_R, DRAW_COLOR_G, DRAW_COLOR_B, DRAW_COLOR_A);
    if(0 > SDL_RenderFillRect(sorter.mainRenderer, &border)) {
	SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
	return false;
    }
    */

    return true;
}

void setColorClear(struct visualSorter sorter) {
    SDL_SetRenderDrawColor(sorter.mainRenderer, CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A);
}

void setColorDraw(struct visualSorter sorter) {
    SDL_SetRenderDrawColor(sorter.mainRenderer, DRAW_COLOR_R, DRAW_COLOR_G, DRAW_COLOR_B, DRAW_COLOR_A);
}

bool drawState(struct visualSorter sorter, int *vals, int size) {
    int bottom = SCREEN_HEIGHT - CUSHION;
    int i = 0;
    struct bar bar = sorter.bar;

    setColorDraw(sorter);

    //for each value in vals, draw rectangle
    for(i = 0; i < size; ++i) {
	int barHeight = vals[i] * bar.heightRes;
	SDL_Rect barRect = {bar.frontPorch + (i * bar.stride), bottom - barHeight, bar.width, barHeight};
	if(0 > SDL_RenderFillRect(sorter.mainRenderer, &barRect)) {
	    SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
	    return false;
	}

    }
    return true;
}

/*
bool drawAllStates(struct visualSorter sorter) {
   //todo: 
   //do

   //actually, pass in starter values once at beginning
    if (!drawState(sorter, vals, ARRAY_SIZE)) {
	return false;
    }

    return true;
}
*/

bool drawScreen(struct visualSorter sorter) {
    int i = 0;
    if(!clearScreen(sorter)) {
	return false;
    }

    if(!drawBorders(sorter)) {
	return false;
    }

    for (i = 0; i < sorter.numAlgs; ++i) {
	struct algContext alg = sorter.algorithms[i];
	drawState(sorter, alg.ep.getData(alg.privateData), alg.ep.getDataSize(alg.privateData));
    }

    //todo draw the states in a loop
    /*
    if(!drawAllStates(sorter)) {
	return false;
    }
    */

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

void setLayout(struct layout *layout) {
    layout->numTiles = 2;
    layout->rows = 1;
    layout->cols = 2;
}

void setBar(struct bar *bar) {
    bar->width = ((SCREEN_WIDTH - CUSHION)/ARRAY_SIZE) - CUSHION ;
    SDL_Log("bar width = %d", bar->width);
    bar->heightRes = (SCREEN_HEIGHT - (2 * CUSHION)) / (VAL_MAX);
    SDL_Log("bar heightRes = %d", bar->heightRes);
    bar->stride = bar->width + CUSHION;
    SDL_Log("bar stride = %d", bar->stride);
    bar->frontPorch = (SCREEN_WIDTH - (ARRAY_SIZE * bar->stride))/2;
    SDL_Log("bar front porch = %d", bar->frontPorch);
}

int alg_init(struct visualSorter *sorter, int *data, int dataSize) {
    int i = 0;
    struct algDesc *desc = list;

    sorter->numAlgs = 0;

    while(desc->add != NULL) {
	sorter->numAlgs++;
	desc++;
    }

    SDL_Log("number of Algorithms found: %d", sorter->numAlgs);

    sorter->algorithms = (struct algContext *) malloc(sizeof(struct algContext) * sorter->numAlgs);
    if (sorter->algorithms  == NULL) {
	SDL_Log("alg_init: malloc failed");
	return -1;
    }

    for (i = 0; i < sorter->numAlgs; ++i) {
	list[i].add(&sorter->algorithms[i].ep);
	sorter->algorithms[i].privateData = sorter->algorithms[i].ep.init(data, dataSize);
    }

    return 0;
}

void alg_deinit(struct algContext *algorithms, int count) {
    int i = 0;
    for (i = 0; i < count; ++i) {
	algorithms->ep.deinit(algorithms->privateData);
	algorithms->privateData = NULL;
    }

    free(algorithms);
}

void initData(int *data, int dataSize, enum initialState state) {
    int i = 0;

    switch (state) {
    default:
	SDL_Log("InitData: defaulting");

    //intentional fallthrough
    case SORTED:
	SDL_Log("InitData: SORTED dataSize %d", dataSize);
       for (i = 0; i < dataSize; ++i) {
	    data[i] = i % (dataSize + 1) + 1;
       }
       break;

    case REVERSE_SORTED:
	SDL_Log("InitData: REVERSE_SORTED");
       break;

    case RANDOM:
	SDL_Log("InitData: RANDOM");
       break;
    }
}

int main(int argc, char* argv[]) {
    struct visualSorter sorter = {NULL, NULL, {0,0,0}};
    bool quit = false;
    int data [ARRAY_SIZE];
    int dataSize = ARRAY_SIZE;

    //intialized SDL, create window
    if(!init(&sorter)) {
	SDL_Log("Error initializing SDL");
	quit = true;
    }

    setLayout(&sorter.layout);
    setBar(&sorter.bar);

    initData(data, dataSize, SORTED);

    if (0 > alg_init(&sorter, data, dataSize)) {
	SDL_Log("failed to initialize algorithms");
	quit = true;
    }

    while(!quit) {
	quit = loopHandler(sorter);
    }

    alg_deinit(sorter.algorithms, sorter.numAlgs);
    sorter.algorithms = NULL;
    cleanup(&sorter);
    return 0;
}

