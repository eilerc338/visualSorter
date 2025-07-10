#include "algorithm.h"
#include "dataMgr.h"

#include <vector>
#include <thread>

class insertionSort : public algorithm {
	public:
		insertionSort() = delete;
		~insertionSort();
		insertionSort(std::vector<int> initialData);
		void doNextStep() override;
		//const std::vector<int>& getData() override;
		int getNumSteps() override;
		bool isComplete() override;
		static void sort(insertionSort& sort);
		size_t getLength();
		const std::vector<int>& getData();

	private:
		bool complete = false;
		std::thread sortThread;
		dataMgr dMgr;
};
