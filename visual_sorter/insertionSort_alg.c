#include "SDL.h"
#include <pthread.h>
#include <stdbool.h>
#include "sort_algorithms.h"

struct insertionContext {
    int *data;
    size_t dataSize;
    bool finished;
    pthread_t thread;
};

void insertion_deinit(void *context) {
    struct insertionContext *ins = (struct insertionContext*) context;
    free(ins->data);
    free(ins);
    return;
}

void insertion_doStep(void *privateData) {
    struct insertionContext *context = (struct insertionContext *) privateData;
    SDL_Log("insertion_doStep: enter");
}

void* insertion_sort(void *privateData) {
    struct insertionContext *context = (struct insertionContext *) privateData;
    int i = 1;
    SDL_Log("insertion_sort: enter");
    /*

    for (i = 1; i < context->dataSize; ++i) {
	int j = i-1;
	while(j > 0 && context->data[j+1] < context->data[j]) {
	    int tmp = context->data[j+1];
	    context->data[j+1] = context->data[j];
	    context->data[j];
	}
    }
    context->finished = true;
    */
    SDL_Log("insertion_sort finished");
    return NULL;
}

int* insertion_getData(void *privateData) {
    struct insertionContext *context = (struct insertionContext *) privateData;

    return context->data;
}

size_t insertion_getDataSize(void *privateData) {
    struct insertionContext *context = (struct insertionContext *) privateData;

    return context->dataSize;
}

void* insertion_init(int *data, size_t dataSize) {
    struct insertionContext *context;
    SDL_Log("insertion_init: enter");

    context = (struct insertionContext *) malloc(sizeof(struct insertionContext));
    context->data = (int*) malloc(sizeof(int) * dataSize);

    memcpy(context->data, data, sizeof(int) * dataSize);
    context->dataSize = dataSize;
    context->finished = false;

    if (pthread_create(&context->thread, NULL, &insertion_sort, context)){
	SDL_Log("pthread_create error");
    }

    SDL_Log("insertion_init: exit");
    return (void *) context;
}

bool insertionAdd(struct entryPoints *ep) {
    if(ep == NULL) {
	return false;
    }

    ep->init = &insertion_init;
    ep->deinit = &insertion_deinit;
    ep->doStep = &insertion_doStep;
    ep->getData = &insertion_getData;
    ep->getDataSize = &insertion_getDataSize;
    return true;
}

