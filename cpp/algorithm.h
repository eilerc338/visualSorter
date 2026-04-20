#pragma once

#include "dataMgr.h"
#include "windowManager.h"

#include <iostream>
#include <vector>
#include <thread>

class algorithm {
	public:
		~algorithm() {
			std::cout << "~algorithm()" << std::endl;
			exit_thread = true;
			dMgr.deInit();
			sortThread.join();
		}

		algorithm(std::vector<int> initialData) {
			dMgr.init(initialData);
		}

		virtual std::string_view getName() const = 0;

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

		virtual void setSurface(const surface_t *surf) {
			m_surface = surf;
		}

	protected:
		dataMgr dMgr;
		bool complete = false;
		std::thread sortThread;
		bool exit_thread = false;
		const surface_t *m_surface;
};

class insertionSort : public algorithm {
	public:
		insertionSort() = delete;
		insertionSort(std::vector<int> initialData);
		static void startSort(insertionSort& sort);
		void sort();
		std::string_view getName() const override {
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
		std::string_view getName() const override {
			return name;
		}

	private:
		std::string_view name = "Merge Sort";
};
