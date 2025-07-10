#include "algorithm.h"
#include <iostream>

mergeSort::mergeSort(std::vector<int> initialData): algorithm(initialData) {
	std::thread t1(mergeSort::startSort, std::ref(*this));
	sortThread = std::move(t1);
}

void mergeSort::merge(int start, int middle, int end) {
	int size = end - start + 1;
	int tmp;
	std::vector<int> tmpVector;
	int i = 0;
	int j = 0;
	int k = 0;

	//SDL_Log("merge: start %d, middle %d, end %d, size %d", start, middle, end, size);

	middle = middle - start;
	k = middle + 1;

	for (i = 0; i < size; ++i) {
		if (exit_thread) {
			return;
		}
		dMgr.get(start + i, &tmp);
		tmpVector.push_back(tmp);
	}

	i = start;
	while (i < (start + size)) {
		//SDL_Log("merge: i %d, j %d, k %d, size %d", i, j, k, size);
		if (exit_thread) {
			return;
		}
		if (j > middle) {
			dMgr.store(i++, tmpVector[k++]);
		} else if (k >= size) {
			dMgr.store(i++, tmpVector[j++]);
		} else {
			if (tmpVector[j] <= tmpVector[k]) {
				dMgr.store(i++, tmpVector[j++]);
			} else {
				dMgr.store(i++, tmpVector[k++]);
			}
		}
	}
	//SDL_Log("merge: exit");
}

void mergeSort::sort(int start, int end) {
	int middle = 0;
	if (start >= end) {
		return;
	}

	middle = ((end - start) / 2) + start;
	sort(start, middle);
	sort(middle+1, end);
	merge(start, middle, end);
}

void mergeSort::startSort(mergeSort& sort) {
	sort.sort(0, sort.dMgr.getLength()-1);
	std::cout << "mergeSort: done" << std::endl;
	sort.complete = true;
}
