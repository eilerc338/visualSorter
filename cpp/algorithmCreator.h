#pragma once

#include "algorithm.h"
#include <vector>
#include <memory>

class algorithmCreator {
	public:
		virtual std::unique_ptr<algorithm> createAlgorithm(std::vector<int> initialData) = 0;
};
