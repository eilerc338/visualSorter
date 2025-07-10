#include "algorithm.h"
#include <iostream>

insertionSort::~insertionSort() {
	sortThread.join();
}

insertionSort::insertionSort(std::vector<int> initialData): algorithm(initialData) {
	std::cout << "insertionSort constructor (initialData)" << std::endl;
	std::thread t1(insertionSort::sort, std::ref(*this));
	sortThread = std::move(t1);

}

void insertionSort::sort(insertionSort& sort) {
	std::cout << "insertionSort thread started" << std::endl;
	dataMgr& dMgr = sort.dMgr;
	for (size_t i = 1; i < dMgr.getLength(); ++i) {
		int j = i-1;
		int data_j;
		int data_jplus1;

		dMgr.get(j, &data_j);
		dMgr.get(j+1, &data_jplus1);

		while (j >= 0 && (data_jplus1 < data_j)) {
			dMgr.store(j, data_jplus1);
			dMgr.store(j+1, data_j);
			j--;

			if (j >= 0) {
				dMgr.get(j, &data_j);
				dMgr.get(j+1, &data_jplus1);
			}
		}
	}

	std::cout << "insertionSort: done" << std::endl;
	sort.complete = true;
}
