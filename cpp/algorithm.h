#pragma once

#include "dataMgr.h"

#include <iostream>
#include <vector>

class algorithm {
	public:
		//algorithm() = delete;
		//~algorithm() = default;
		algorithm(std::vector<int> initialData) {
			std::cout << "algorithim constructor (initialData)" << std::endl;
			dMgr.init(initialData);
		}

		virtual std::string_view getName() = 0;

		virtual void doNextStep() {
			dMgr.release();
		}

		virtual int getNumSteps() {
			return dMgr.getNumSteps();
		}

		virtual bool isComplete() {
			return complete;
		}

		virtual const std::vector<int>& getData() {
			return dMgr.getData();
		}

	protected:
		dataMgr dMgr;
		bool complete = false;
};
