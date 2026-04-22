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
		bool isWaiting();
		size_t getLength();
		int getMin() {return m_minV;}
		int getMax() {return m_maxV;}

		~dataMgr();

	private:
		bool wait();

		bool m_waiting = false;
		unsigned int numSteps = 0;
		std::vector<int> data;
		std::mutex mutex;
		std::condition_variable cond;
		bool exit_thread = false;
		int m_minV = 0;
		int m_maxV = 0;
};
