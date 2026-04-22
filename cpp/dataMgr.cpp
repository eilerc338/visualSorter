#include "dataMgr.h"

#include <iostream>

int dataMgr::init(std::vector<int>& initialData)
{
	data.clear();
	data = initialData;
	m_minV = data[0];

	for (auto &d : data) {
		if (d < m_minV) {
			m_minV = d;
		}

		if (d > m_maxV) {
			m_maxV = d;
		}
	}
	return 0;
}

bool dataMgr::isWaiting()
{
	return m_waiting;
}

bool dataMgr::wait()
{
	m_waiting = true;
	if (!exit_thread) {
		std::unique_lock lock(mutex);
		cond.wait(lock);
		return true;
	} else {
		m_waiting = true;
	}
	return false;
}

int dataMgr::get(unsigned int pos, int *value )
{
	if (wait()) {
		*value = data[pos];
	}
	return 0;
}

int dataMgr::store(unsigned int pos, int value)
{
	if (wait()) {
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
	return data;
}

int dataMgr::release()
{
	m_waiting = false;
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
