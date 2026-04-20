#include "algorithmCreator.h"
#include "visualSorter.h"

bool visualSorter::mainLoop()
{
    SDL_Event e;

	while(SDL_PollEvent(&e) != 0) {
		//user requests to quit
		if(e.type == SDL_QUIT) {
			return false;
		}
	}

	updateScreen();
	return true;
}

void visualSorter::updateScreen()
{
	winMgr.updateScreen();
}

void visualSorter::drawBorders()
{
	winMgr.drawBorders();
}

bool visualSorter::initWindow()
{
	return winMgr.init(getNumAlgorithms());
}

int visualSorter::initAlgorithms(std::vector<int> &initialData)
{
	insertionCreator insCreator;
	mergeCreator mrgCreator;

	addAlgorithm(insCreator, initialData);
	addAlgorithm(mrgCreator, initialData);

	return 0;
}

int visualSorter::addAlgorithm(algorithmCreator &algCreator, std::vector<int> &initialData)
{
	algorithms.push_back(algCreator.createAlgorithm(initialData));
	return 0;
}

bool visualSorter::getSurfaces()
{
	for (const auto &a : algorithms) {
		const surface_t *surf = winMgr.getSurface();
		if (!surf) {
			return false;
		}
		a->setSurface(surf);
	}

	return true;
}

unsigned int visualSorter::getNumAlgorithms()
{
	return algorithms.size();
}

bool visualSorter::isAllComplete()
{
	for (const auto &a : algorithms) {
		if (!a->isComplete()) {
			return false;
		}
	}
	return true;
}

void visualSorter::debugPrintData()
{
	std::cout << std::endl;
	for (const auto &a : algorithms) {
		std::cout << a->getName() << ": ";
		auto data = a->getData();

		for (auto v : data) {
			std::cout << v << " ";
		}
		std::cout << std::endl;
	}
}

void visualSorter::doStep()
{
	for (const auto &a : algorithms) {
		a->doNextStep();
	}
}
