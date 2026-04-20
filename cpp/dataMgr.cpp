#include "dataMgr.h"

#include <iostream>

int dataMgr::init(std::vector<int>& initialData)
{
	data.clear();
	data = initialData;
	return 0;
}

int dataMgr::get(unsigned int pos, int *value )
{
	if (!exit_thread) {
		std::unique_lock lock(mutex);
		cond.wait(lock);
		*value = data[pos];
	}
	return 0;
}

int dataMgr::store(unsigned int pos, int value)
{
	if (!exit_thread) {
		std::unique_lock lock(mutex);
		cond.wait(lock);

		if (pos < data.size()) {
			data[pos] = value;
		}else {
			data.push_back(value);
		}
	}
	return 0;
}

const std::vector<int>& dataMgr::getData()
{
	std::lock_guard guard(mutex);
	return data;
}

int dataMgr::release()
{
	cond.notify_one();
	++numSteps;
	return 0;
}

int dataMgr::getNumSteps()
{
	return numSteps;
}

size_t dataMgr::getLength()
{
	return data.size();
}

void dataMgr::deInit()
{
	std::cout << "datamgr::deInit()" << std::endl;
	exit_thread = true;
	cond.notify_one();
}

dataMgr::~dataMgr()
{
	std::cout << "~datamgr()" << std::endl;
	exit_thread = true;
	cond.notify_one();
}
