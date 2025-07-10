#pragma once

#include "algorithm.h"
#include <vector>
#include <memory>

class algorithmCreator {
	public:
		virtual std::unique_ptr<algorithm> createAlgorithm(std::vector<int> initialData) = 0;
};

class insertionCreator : public algorithmCreator {
	public:
		std::unique_ptr<algorithm> createAlgorithm(std::vector<int> initialData) override {
			return std::make_unique<insertionSort>(initialData);
		}
};

class mergeCreator : public algorithmCreator {
	public:
		std::unique_ptr<algorithm> createAlgorithm(std::vector<int> initialData) override {
			return std::make_unique<mergeSort>(initialData);
		}
};
