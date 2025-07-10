#include "algorithm.h"
#include <iostream>

mergeSort::~mergeSort() {
	sortThread.join();
}

mergeSort::mergeSort(std::vector<int> initialData): algorithm(initialData) {
	std::thread t1(mergeSort::sort, std::ref(*this));
	sortThread = std::move(t1);

}

void mergeSort::sort(mergeSort& sort) {
	dataMgr& dMgr = sort.dMgr;

	std::cout << "mergeSort: done" << std::endl;
	sort.complete = true;
}
