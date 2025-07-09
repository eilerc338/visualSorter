#pragma once

#include <iostream>
#include <vector>

class algorithm {
	public:
		virtual void doNextStep() = 0;
		virtual int getNumSteps() = 0;
		virtual bool isComplete() = 0;
		virtual ~algorithm() = default;

		virtual const std::vector<int>& getData() {
			std::cout << "insertionSort::getData()" << std::endl;
			return data;
		}


	protected:
		std::vector<int> data {};
};
