
#include "MotionClassRecognizer.h"
#include <time.h>
#include <algorithm>

const double MotionClassRecognizer::RECOGNIZE_LIMIT = 0.5;
const int MIN_COMMON_COMPARE_SIZE = 5;

MotionClassRecognizer::MotionClassRecognizer(const std::string motionClassName, const MotionObject learnedData, const MotionObject recognizingData)
	: motionClassName(motionClassName), learnedData(learnedData), recognizingData(recognizingData) {
	createComparators();
}

MotionClassRecognizer::~MotionClassRecognizer() {}

void MotionClassRecognizer::createComparators() {
	int minOffset = 1 - learnedData.size() + std::min((int)learnedData.size()-1, MIN_COMMON_COMPARE_SIZE);
	int maxOffset = recognizingData.size() - 1 - std::min((int)learnedData.size() - 1, MIN_COMMON_COMPARE_SIZE);

	std::cout << "Creating MotionComparators for " << this->motionClassName << ". " << learnedData.size() << " " << recognizingData.size() << std::endl;
	const clock_t begin_time = clock();
	for (int offset = minOffset; offset <= maxOffset; offset++) {
		std::cout << "Created " << offset - minOffset << "/" << maxOffset - minOffset << " from " << offset << '\xd';
		this->comparators.push(MotionComparator(offset, learnedData, recognizingData));
	}
	std::cout << std::endl << "average create time = " << float(clock() - begin_time) / CLOCKS_PER_SEC / (maxOffset - minOffset + 1) << std::endl;
}

std::vector<MotionComparator> MotionClassRecognizer::getBestComparators() {
	return bestComparators;
	/*
	std::vector<MotionComparator> toReturn;

	std::cout << "Comparing " << this->motionClassName << std::endl;
	while (!comparators.empty()) {
		auto comparator = comparators.top();
		if (comparator.getSimilarity() > RECOGNIZE_LIMIT) {
			toReturn.push_back(comparator);
		}
		comparators.pop();
		std::cout << " comparator similarity = " << comparator.getSimilarity() << std::endl;
	}
	return toReturn;
	*/
}

std::string MotionClassRecognizer::getClassName() {
	return this->motionClassName;
}

bool MotionClassRecognizer::compareFrames() {
	std::cout << "Comparing " << this->motionClassName << std::endl;
	bool goOn;
	do {
		goOn = false;
		auto comparator = comparators.top();
		comparators.pop();
		goOn = comparator.compareNextFrame();
		if (!goOn && comparator.getSimilarity() > RECOGNIZE_LIMIT) {
			bestComparators.push_back(comparator);
			goOn = true;
		} else {
			comparators.push(comparator);
		}
	} while (goOn);

	std::cout << "best comparators:" << std::endl;
	for (auto comparator: bestComparators) {
		std::cout << "\tsimilarity = " << comparator.getSimilarity() << std::endl;
	}

	std::cout << "the others:" << std::endl;
	while (!comparators.empty()) {
		auto comparator = comparators.top();
		comparators.pop();
		std::cout << "\tsimilarity = " << comparator.getSimilarity() << std::endl;
	}
	return false;
}
