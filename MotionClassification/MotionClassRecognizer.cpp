
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

	std::clog << "Creating MotionComparators for " << this->motionClassName << ". " << learnedData.size() << " " << recognizingData.size() << "\n";
	const clock_t begin_time = clock();
	for (int offset = minOffset; offset <= maxOffset; offset++) {
		std::clog << "Created " << offset - minOffset << "/" << maxOffset - minOffset << " from " << offset << '\xd';
		this->comparators.push(MotionComparator(offset, learnedData, recognizingData));
	}
	std::clog << "\n" << "average create time = " << float(clock() - begin_time) / CLOCKS_PER_SEC / (maxOffset - minOffset + 1) << "\n";
}

std::vector<MotionComparator> MotionClassRecognizer::getBestComparators() {
	return bestComparators;
	/*
	std::vector<MotionComparator> toReturn;

	std::clog << "Comparing " << this->motionClassName << "
";
	while (!comparators.empty()) {
		auto comparator = comparators.top();
		if (comparator.getSimilarity() > RECOGNIZE_LIMIT) {
			toReturn.push_back(comparator);
		}
		comparators.pop();
		std::clog << " comparator similarity = " << comparator.getSimilarity() << "
";
	}
	return toReturn;
	*/
}

std::string MotionClassRecognizer::getClassName() {
	return this->motionClassName;
}

bool MotionClassRecognizer::compareFrames() {
	std::clog << "Comparing " << this->motionClassName << "\n";
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

	std::clog << "best comparators:" << "\n";
	for (auto comparator: bestComparators) {
		std::clog << "\tsimilarity = " << comparator.getSimilarity() << "\n";
	}

	std::clog << "the others:" << "\n";
	while (!comparators.empty()) {
		auto comparator = comparators.top();
		comparators.pop();
		std::clog << "\tsimilarity = " << comparator.getSimilarity() << "\n";
	}
	return false;
}
