#pragma once

#include "algorithmCreator.h"

class visualSorter {
	public:
		std::vector<std::unique_ptr<algorithm>> algorithms {};

	public:
		int addAlgorithm(algorithmCreator &algCreator, std::vector<int> &initialData) {
			algorithms.push_back(algCreator.createAlgorithm(initialData));

			return 0;
		}

		unsigned int getNumAlgorithms() {
			return algorithms.size();
		}

		bool isAllComplete() {
			for (const auto &a : algorithms) {
				if (!a->isComplete()) {
					return false;
				}
			}
			return true;
		}

		void debugPrintData() {
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

		void doStep() {
			for (const auto &a : algorithms) {
				a->doNextStep();
			}
		}
};
