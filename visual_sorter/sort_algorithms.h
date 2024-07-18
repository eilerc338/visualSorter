#ifndef __SORT_ALGORITHMS_H__
#define __SORT_ALGORITHMS_H__

 struct entryPoints {
    void* (*init)(int *data, size_t dataSize);
    void (*doStep)(void *privateData);
    int* (*getData)(void *privateData);
    size_t (*getDataSize)(void *privateData);
    void (*deinit)(void *context);
};

bool insertionAdd(struct entryPoints *ep);

#endif //__SORT_ALGORITHMS_H__

