#pragma once

#include <vector>

class algorithm {
	public:
		virtual void doNextStep() = 0;
		virtual const std::vector<int>& getData() = 0;
		virtual int getNumSteps() = 0;
		virtual bool isComplete() = 0;
		virtual ~algorithm() = default;

	protected:
		std::vector<int> data {};
};
