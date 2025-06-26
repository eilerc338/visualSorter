#include "SDL.h"
#include <pthread.h>
#include <stdbool.h>
#include "sort_algorithms.h"

struct insertionContext {
    int *data;
    size_t dataSize;
    bool finished;
    bool exit_thread;
    pthread_t thread;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};

void insertion_doStep(void *privateData) {
    struct insertionContext *context = (struct insertionContext *) privateData;
    pthread_cond_signal(&context->cond);
}

void* insertion_sort(void *privateData) {
    struct insertionContext *context = (struct insertionContext *) privateData;
    int i = 1;
    SDL_Log("insertion_sort: enter");

    pthread_mutex_lock(&context->mutex);

    for (i = 1; i < context->dataSize; ++i) {
	int j = i-1;
	while (j >= 0 && (context->data[j+1] < context->data[j])) {
	    int tmp;
	    if (context->exit_thread) {
		pthread_mutex_unlock(&context->mutex);
		SDL_Log("insertion_sort: exit_thread");
		return NULL;
	    }
	    //SDL_Log("insertion_sort: thread waiting");
	    pthread_cond_wait(&context->cond, &context->mutex);
	    //SDL_Log("insertion_sort: thread released");
	    tmp = context->data[j+1];
	    context->data[j+1] = context->data[j];
	    context->data[j] = tmp;
	    j--;
	}
    }

    context->finished = true;
    pthread_mutex_unlock(&context->mutex);

    SDL_Log("insertion_sort finished");
    return NULL;
}

int insertion_getData(void *privateData, int *dataOut, int *dataSize) {
    struct insertionContext *context = (struct insertionContext *) privateData;
    int ret = 0;

    pthread_mutex_lock(&context->mutex);

    if (*dataSize >= context->dataSize) {
	memcpy(dataOut, context->data, sizeof(int) * context->dataSize);
    } else {
	ret = -1;
    }

    pthread_mutex_unlock(&context->mutex);

    *dataSize = context->dataSize;
    return ret;
}

size_t insertion_getDataSize(void *privateData) {
    struct insertionContext *context = (struct insertionContext *) privateData;

    return context->dataSize;
}

void insertion_deinit(void *privateData) {
    struct insertionContext *context = (struct insertionContext*) privateData;
    context->exit_thread = true;
    SDL_Log("contextertion sort: signal thread for deinit");
    pthread_cond_signal(&context->cond);
    if (0 != pthread_join(context->thread, NULL)) {
	SDL_Log("contextertion sort: thread failed to join");
    }
    pthread_mutex_destroy(&context->mutex);
    pthread_cond_destroy(&context->cond);

    free(context->data);
    free(context);
    return;
}

void* insertion_init(int *data, size_t dataSize) {
    struct insertionContext *context;
    SDL_Log("insertion_init: enter");

    context = (struct insertionContext *) malloc(sizeof(struct insertionContext));
    context->data = (int*) malloc(sizeof(int) * dataSize);

    memcpy(context->data, data, sizeof(int) * dataSize);
    context->dataSize = dataSize;
    context->finished = false;
    context->exit_thread = false;

    if (pthread_cond_init(&context->cond, NULL)) {
	SDL_Log("insertion sort: failed to init condition variable");
	goto dealloc;
    }

    if (pthread_mutex_init(&context->mutex, NULL)) {
	SDL_Log("insertion sort: failed to init mutex");
	goto dealloc;
    }

    if (pthread_create(&context->thread, NULL, &insertion_sort, context)) {
	SDL_Log("pthread_create error");
	goto dealloc;
    }

    SDL_Log("insertion_init: exit");
    return (void *) context;

dealloc:
    insertion_deinit(context);
    return NULL;
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

