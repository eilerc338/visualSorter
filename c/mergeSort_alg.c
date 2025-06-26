#include "SDL.h"
#include <pthread.h>
#include <stdbool.h>
#include "sort_algorithms.h"

struct mergeContext {
    int *data;
    size_t dataSize;
    bool finished;
    bool exit_thread;
    pthread_t thread;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};

void merge_doStep(void *privateData) {
    struct mergeContext *context = (struct mergeContext *) privateData;
    pthread_cond_signal(&context->cond);
}

void merge(struct mergeContext *context, int start, int middle, int end) {
    int size = end - start + 1;
    int temparr[size];
    int i = 0;
    int j = 0;
    int k = 0;

    //SDL_Log("merge: start %d, middle %d, end %d, size %d", start, middle, end, size);
    
    middle = middle - start;
    k = middle + 1;

    //memcpy(temparr, &context->data[start], sizeof(int) * size);
    for (i = 0; i < size; ++i) {
	if (context->exit_thread) {
	    return;
	}
	pthread_cond_wait(&context->cond, &context->mutex);
	temparr[i] = context->data[start + i];
    }

    i = start;
    while (i < (start + size)) {
	//SDL_Log("merge: i %d, j %d, k %d, size %d", i, j, k, size);
	if (context->exit_thread) {
	    return;
	}
	pthread_cond_wait(&context->cond, &context->mutex);
	if (j > middle) {
	    context->data[i++] = temparr[k++];
	} else if (k >= size) {
	    context->data[i++] = temparr[j++];
	} else {
	    if (temparr[j] <= temparr[k]) {
		context->data[i++] = temparr[j++];
	    } else {
		context->data[i++] = temparr[k++];
	    }
	}
    }
    //SDL_Log("merge: exit");
}

void mergeHelper(struct mergeContext *context, int start, int end) {
    int middle = 0;
    if (start >= end) {
	return;
    }

    middle = ((end - start) / 2) + start;
    mergeHelper(context, start, middle);
    mergeHelper(context, middle+1, end);
    merge(context, start, middle, end);
}

void* merge_sort(void *privateData) {
    struct mergeContext *context = (struct mergeContext *) privateData;
    //SDL_Log("merge_sort: enter");

    pthread_mutex_lock(&context->mutex);

    mergeHelper(context, 0, context->dataSize-1);
    context->finished = true;
    pthread_mutex_unlock(&context->mutex);

    //SDL_Log("merge_sort finished");
    return NULL;
}

int merge_getData(void *privateData, int *dataOut, int *dataSize) {
    struct mergeContext *context = (struct mergeContext *) privateData;
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

size_t merge_getDataSize(void *privateData) {
    struct mergeContext *context = (struct mergeContext *) privateData;

    return context->dataSize;
}

void merge_deinit(void *privateData) {
    struct mergeContext *context = (struct mergeContext*) privateData;
    context->exit_thread = true;
    SDL_Log("merge sort: signal thread for deinit");
    pthread_cond_signal(&context->cond);
    if (0 != pthread_join(context->thread, NULL)) {
	SDL_Log("merge sort: thread failed to join");
    }
    pthread_mutex_destroy(&context->mutex);
    pthread_cond_destroy(&context->cond);

    free(context->data);
    free(context);
    return;
}

void* merge_init(int *data, size_t dataSize) {
    struct mergeContext *context;
    //SDL_Log("merge_init: enter");

    context = (struct mergeContext *) malloc(sizeof(struct mergeContext));
    context->data = (int*) malloc(sizeof(int) * dataSize);

    memcpy(context->data, data, sizeof(int) * dataSize);
    context->dataSize = dataSize;
    context->finished = false;
    context->exit_thread = false;

    if (pthread_cond_init(&context->cond, NULL)) {
	SDL_Log("merge sort: failed to init condition variable");
	goto dealloc;
    }

    if (pthread_mutex_init(&context->mutex, NULL)) {
	SDL_Log("merge sort: failed to init mutex");
	goto dealloc;
    }

    if (pthread_create(&context->thread, NULL, &merge_sort, context)) {
	SDL_Log("pthread_create error");
	goto dealloc;
    }

    //SDL_Log("merge_init: exit");
    return (void *) context;

dealloc:
    merge_deinit(context);
    return NULL;
}

bool mergeAdd(struct entryPoints *ep) {
    if(ep == NULL) {
	return false;
    }

    ep->init = &merge_init;
    ep->deinit = &merge_deinit;
    ep->doStep = &merge_doStep;
    ep->getData = &merge_getData;
    ep->getDataSize = &merge_getDataSize;
    return true;
}

