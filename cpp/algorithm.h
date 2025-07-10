#pragma once

#include "dataMgr.h"

#include <iostream>
#include <vector>
#include <thread>

class algorithm {
	public:
		~algorithm() {
			exit_thread = true;
			sortThread.join();
		}

		algorithm(std::vector<int> initialData) {
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
		std::thread sortThread;
		bool exit_thread = false;
};

class insertionSort : public algorithm {
	public:
		insertionSort() = delete;
		insertionSort(std::vector<int> initialData);
		static void startSort(insertionSort& sort);
		void sort();
		std::string_view getName() override {
			return name;
		}

	private:
		std::string_view name = "Insertion Sort";
};

class mergeSort : public algorithm {
	public:
		mergeSort() = delete;
		mergeSort(std::vector<int> initialData);
		void merge(int start, int middle, int end);
		void sort(int start, int end);
		static void startSort(mergeSort& sort);
		std::string_view getName() override {
			return name;
		}

	private:
		std::string_view name = "Merge Sort";
};
