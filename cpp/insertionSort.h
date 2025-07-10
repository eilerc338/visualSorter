#pragma once

#include "algorithm.h"

#include <vector>
#include <thread>

constexpr std::string_view name = "Insertion Sort";

class insertionSort : public algorithm {
	public:
		insertionSort() = delete;
		~insertionSort();
		insertionSort(std::vector<int> initialData);
		static void sort(insertionSort& sort);
		std::string_view getName() override {
			return name;
		}

	private:
		std::thread sortThread;
};
