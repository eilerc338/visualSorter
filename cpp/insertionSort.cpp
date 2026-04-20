#include "algorithm.h"
#include <iostream>

void insertionSort::startSort(insertionSort& sort) {
	sort.sort();
}

void insertionSort::sort() {
	//dataMgr& dMgr = sort.dMgr;
	for (size_t i = 1; i < dMgr.getLength(); ++i) {
		int j = i-1;
		int data_j;
		int data_jplus1;

		dMgr.get(j, &data_j);
		dMgr.get(j+1, &data_jplus1);

		while (j >= 0 && (data_jplus1 < data_j)) {
			if (exit_thread) {
				return;
			}
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
	complete = true;
}

//constructors / desctructors
insertionSort::insertionSort(std::vector<int> initialData): algorithm(initialData) {
	std::thread t1(insertionSort::startSort, std::ref(*this));
	sortThread = std::move(t1);
}
