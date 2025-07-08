#include "algorithm.h"
#include <vector>

class insertionSort : public algorithm {
	public:
		insertionSort(std::vector<int> initialData);
		void doNextStep() override;
		const std::vector<int>& getData() override;
		int getNumSteps() override;
		bool isComplete() override;
};
