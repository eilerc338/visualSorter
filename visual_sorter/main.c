#include "SDL.h"
#include <stdbool.h>
#include "sort_algorithms.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

//MAIN color
#define MAIN_COLOR_R 0xFF
#define MAIN_COLOR_G 0xFF
#define MAIN_COLOR_B 0xFF
#define MAIN_COLOR_A SDL_ALPHA_OPAQUE

//red color
#define RED_COLOR_R 0xFF
#define RED_COLOR_G 0x00
#define RED_COLOR_B 0x00
#define RED_COLOR_A SDL_ALPHA_OPAQUE

//green color
#define GREEN_COLOR_R 0x00
#define GREEN_COLOR_G 0xFF
#define GREEN_COLOR_B 0x00
#define GREEN_COLOR_A SDL_ALPHA_OPAQUE

//blue color
#define BLUE_COLOR_R 0x00
#define BLUE_COLOR_G 0x00
#define BLUE_COLOR_B 0xFF
#define BLUE_COLOR_A SDL_ALPHA_OPAQUE

//clear color
#define CLEAR_COLOR_R 0x00
#define CLEAR_COLOR_G 0x00
#define CLEAR_COLOR_B 0x00
#define CLEAR_COLOR_A SDL_ALPHA_OPAQUE

#define CUSHION		2	//pixels between bars
#define BORDER_WIDTH	6
#define BUMPER 		((BORDER_WIDTH/2) + CUSHION)
#define ARRAY_SIZE 	120
#define VAL_MAX 	ARRAY_SIZE

enum initialState {
    SORTED,
    REVERSE_SORTED,
    RANDOM,
};

enum color {
    CLEAR,
    MAIN,
    RED,
    GREEN,
    BLUE,
};

struct algDesc {
    char friendlyName[FILENAME_MAX];
    bool (*add)(struct entryPoints *ep);
};

struct algDesc list[] = {
    {"Insertion Sort", &insertionAdd},
    {"", NULL}
};

struct algContext {
    int tileStartX;
    int tileStartY;
    struct entryPoints ep;
    void *privateData;
};

struct layout {
    //int numTiles;
    int rows;
    int cols;
    int tileWidth;
    int tileHeight;
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

    //pointer to a malloced array
    struct algContext *algorithms;
    int numAlgs;
};

void setColor(struct visualSorter sorter, enum color color) {
    //todo add different color options in switch
    switch (color) {
    case CLEAR:
	SDL_SetRenderDrawColor(sorter.mainRenderer, CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A);
	break;

    case MAIN:
	SDL_SetRenderDrawColor(sorter.mainRenderer, MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B, MAIN_COLOR_A);
	break;

    case RED:
	SDL_SetRenderDrawColor(sorter.mainRenderer, RED_COLOR_R, RED_COLOR_G, RED_COLOR_B, RED_COLOR_A);
	break;

    case GREEN:
	SDL_SetRenderDrawColor(sorter.mainRenderer, GREEN_COLOR_R, GREEN_COLOR_G, GREEN_COLOR_B, GREEN_COLOR_A);
	break;

    case BLUE:
	SDL_SetRenderDrawColor(sorter.mainRenderer, BLUE_COLOR_R, BLUE_COLOR_G, BLUE_COLOR_B, BLUE_COLOR_A);
	break;
    }
}

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
    setColor(sorter, CLEAR);
    if(0 > SDL_RenderClear(sorter.mainRenderer)) {
	SDL_Log("SDL_RenderClear: error = %s", SDL_GetError());
	return false;
    }

	return true;
}

SDL_Rect makeRect(int x, int y, int w, int h) {
    SDL_Rect rect = {x, y, w, h};
    return rect;
}

//draw the borders around sort windows
bool drawBorders(struct visualSorter sorter) {
    int i = 0;
    SDL_Rect border;

    setColor(sorter, MAIN);
    //top
    border = makeRect(0, 0, SCREEN_WIDTH, BORDER_WIDTH/2);
    if(0 > SDL_RenderFillRect(sorter.mainRenderer, &border)) {
	SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
	return false;
    }

    //bottom
    border = makeRect(0, SCREEN_HEIGHT-(BORDER_WIDTH/2), SCREEN_WIDTH, BORDER_WIDTH/2);
    if(0 > SDL_RenderFillRect(sorter.mainRenderer, &border)) {
	SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
	return false;
    }

    //left
    border = makeRect(0, 0, BORDER_WIDTH/2, SCREEN_HEIGHT);
    if(0 > SDL_RenderFillRect(sorter.mainRenderer, &border)) {
	SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
	return false;
    }

    //right
    border = makeRect(SCREEN_WIDTH-(BORDER_WIDTH/2), 0, BORDER_WIDTH/2, SCREEN_HEIGHT);
    if(0 > SDL_RenderFillRect(sorter.mainRenderer, &border)) {
	SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
	return false;
    }

    //vertical borders
    for (i = 1; i < sorter.layout.cols; ++i) {
	border = makeRect((i * sorter.layout.tileWidth) - (BORDER_WIDTH/2), 0, BORDER_WIDTH, SCREEN_HEIGHT);
	if(0 > SDL_RenderFillRect(sorter.mainRenderer, &border)) {
	    SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
	    return false;
	}
    }

    //horizontal borders
    for (i = 1; i < sorter.layout.rows; ++i) {
	border = makeRect(0, (i * sorter.layout.tileHeight) - (BORDER_WIDTH)/2, SCREEN_WIDTH, BORDER_WIDTH);
	if(0 > SDL_RenderFillRect(sorter.mainRenderer, &border)) {
	    SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
	    return false;
	}
    }

    return true;
}

bool drawState(struct visualSorter sorter, int startX, int startY, int *vals, int size) {
    struct layout layout = sorter.layout;
    int bottom = layout.tileHeight - BUMPER;
    int i = 0;
    struct bar bar = sorter.bar;

    setColor(sorter, MAIN);

    //for each value in vals, draw rectangle
    for(i = 0; i < size; ++i) {
	int barHeight = vals[i] * bar.heightRes;
	SDL_Rect barRect = {startX + bar.frontPorch + (i * bar.stride), startY + bottom - barHeight, bar.width, barHeight};
	if(0 > SDL_RenderFillRect(sorter.mainRenderer, &barRect)) {
	    SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
	    return false;
	}

    }
    return true;
}

bool drawScreen(struct visualSorter sorter) {
    int i = 0;
    if(!clearScreen(sorter)) {
	return false;
    }

    if(!drawBorders(sorter)) {
        return false;
    }

    //draw the data for each algorithm
    for (i = 0; i < sorter.numAlgs; ++i) {
	struct algContext alg = sorter.algorithms[i];
	int size = alg.ep.getDataSize(alg.privateData);
	int data[size];

	if (0 != alg.ep.getData(alg.privateData, data, &size)) {
	    SDL_Log("failure getting data. Algorithm %s", list[i].friendlyName);
	    return false;
	}

	//SDL_Log("startX %d, startY %d", alg.tileStartX, alg.tileStartY);
	drawState(sorter, alg.tileStartX, alg.tileStartY, data, size);
    }

    return true;
}

bool step(struct visualSorter sorter) {
    int i;

    for (i = 0; i < sorter.numAlgs; ++i) {
	struct algContext alg = sorter.algorithms[i];
	alg.ep.doStep(alg.privateData);
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

    if(!step(sorter)) {
	return true;
    }	

    if(!drawScreen(sorter)) {
	return true;
    }

    //Update Screen
    SDL_RenderPresent(sorter.mainRenderer);

    return false;

}

void setLayout(struct visualSorter *sorter) {
    int i;
    int j;
    int count = 1;
    float targetAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

    sorter->layout.rows = 1;
    sorter->layout.cols = 1;

    //figure out rows and columns using screen resolution and number of algs
    for (i = 1; i < sorter->numAlgs; ++i) {
	int rows = sorter->layout.rows;
	int cols = sorter->layout.cols;

	if (++count > rows * cols) {
	    if (abs(((cols+1) / rows) - targetAspect) <= abs((cols / (rows+1)) - targetAspect)) {
		sorter->layout.cols++;
	    } else {
		sorter->layout.rows++;
	    }
	}
    }

    sorter->layout.tileWidth = (SCREEN_WIDTH / sorter->layout.cols);
    sorter->layout.tileHeight = (SCREEN_HEIGHT / sorter->layout.rows);

    //set start location for tiles
    count = 0;
    for (i = 0; i < sorter->layout.rows; ++i) {
	for (j = 0; j < sorter->layout.cols; ++j) {
	    struct algContext *alg = &sorter->algorithms[count];
	    alg->tileStartX = (j * sorter->layout.tileWidth);
	    alg->tileStartY = (i * sorter->layout.tileHeight);

	    if (++count >= sorter->numAlgs) {
		return;
	    }
	}
    }
}

void setBar(struct visualSorter *sorter) {
    SDL_Log("sorter->layout.tileWidth %d", sorter->layout.tileWidth);
    sorter->bar.width = ((sorter->layout.tileWidth - CUSHION)/ARRAY_SIZE) - CUSHION;
    SDL_Log("sorter->bar.width %d", sorter->bar.width);
    sorter->bar.heightRes = (sorter->layout.tileHeight - (2 * CUSHION)) / (VAL_MAX);
    SDL_Log("sorter->bar.heightRes %d", sorter->bar.heightRes);
    sorter->bar.stride = sorter->bar.width + CUSHION;
    SDL_Log("sorter->bar.stride %d", sorter->bar.stride);
    sorter->bar.frontPorch = (sorter->layout.tileWidth - (ARRAY_SIZE * sorter->bar.stride))/2;
    SDL_Log("sorter->bar.frontPorch %d", sorter->bar.frontPorch);
}

int alg_init(struct visualSorter *sorter, int *data, int dataSize) {
    int i = 0;
    struct algDesc *desc = list;

    sorter->numAlgs = 0;

    while(desc->add != NULL) {
	sorter->numAlgs++;
	desc++;
    }

    SDL_Log("%d Algorithms found: ", sorter->numAlgs);

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
	algorithms[i].ep.deinit(algorithms[i].privateData);
	algorithms[i].privateData = NULL;
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
       for (i = 0; i < dataSize; ++i) {
	    data[i] = ((dataSize-1) - i) % (dataSize + 1) + 1;
       }
       break;

    case RANDOM:
	SDL_Log("InitData: RANDOM");
       break;
    }
}

int main(int argc, char* argv[]) {
    struct visualSorter sorter;// = {NULL, NULL, {0,0,0}};
    bool quit = false;
    int data [ARRAY_SIZE];
    int dataSize = ARRAY_SIZE;

    //intialized SDL, create window
    if(!init(&sorter)) {
	SDL_Log("Error initializing SDL");
	quit = true;
    }


    initData(data, dataSize, REVERSE_SORTED);

    if (0 > alg_init(&sorter, data, dataSize)) {
	SDL_Log("failed to initialize algorithms");
	quit = true;
    }

    setLayout(&sorter);
    setBar(&sorter);

    while(!quit) {
	quit = loopHandler(sorter);
    }

    alg_deinit(sorter.algorithms, sorter.numAlgs);
    sorter.algorithms = NULL;
    cleanup(&sorter);
    return 0;
}

