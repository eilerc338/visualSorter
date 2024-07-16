#include "SDL.h"
#include <stdbool.h>
#include "sort_algorithms.h"

struct insertionContext {
    int *data;
    size_t dataSize;
    int *head;
    int *tail;
};

void* insertion_init(int *data, size_t dataSize) {
    struct insertionContext *context;
    SDL_Log("insertion_init: enter");

    context = malloc(sizeof(struct insertionContext));
    context->data = malloc(sizeof(int) * dataSize);
    memcpy(context->data, data, sizeof(int) * dataSize);
    context->dataSize = dataSize;
    context->head = &data[0];
    context->tail = &data[dataSize -1];

    return (void *) context;
}

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

int* insertion_getData(void *privateData) {
    struct insertionContext *context = (struct insertionContext *) privateData;

    return context->data;
}

size_t insertion_getDataSize(void *privateData) {
    struct insertionContext *context = (struct insertionContext *) privateData;

    return context->dataSize;
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

//todo: create fake do step function 
//have it return the same data

