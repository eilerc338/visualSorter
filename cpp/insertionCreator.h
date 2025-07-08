#pragma once

#include "algorithmCreator.h"
#include "insertionSort.h"
#include <iostream>
#include <vector>

class insertionCreator : public algorithmCreator {
	public:
		std::unique_ptr<algorithm> createAlgorithm(std::vector<int> initialData) override {
			std::cout << "insertionCreator::createAlgorithm" << std::endl;
			return std::make_unique<insertionSort>(initialData);
		}
};
