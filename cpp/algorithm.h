#pragma once

#include "dataMgr.h"

#include <iostream>
#include <vector>
#include <thread>

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
		std::string_view name = "Insertion Sort";
		std::thread sortThread;
};
