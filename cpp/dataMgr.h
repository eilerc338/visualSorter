#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>

class dataMgr {
	public:
		int init(std::vector<int>& initialData);
		void deInit();
		const std::vector<int>& getData();
		int get(unsigned int pos, int *value );
		int store(unsigned int pos, int value);
		int getNumSteps();
		void sort();
		int release();
		size_t getLength();

		~dataMgr();

	private:
		unsigned int numSteps = 0;
		std::vector<int> data;
		std::mutex mutex;
		std::condition_variable cond;
		bool exit_thread = false;
};
