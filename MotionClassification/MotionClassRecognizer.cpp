
#include "MotionClassRecognizer.h"

MotionClassRecognizer::MotionClassRecognizer(const std::string motionClassName, const std::vector<MotionFrame> learnedData, const std::vector<MotionFrame> recognizingData)
	: motionClassName(motionClassName), learnedData(learnedData), recognizingData(recognizingData) {
	createComparators();
}

MotionClassRecognizer::~MotionClassRecognizer() {}

void MotionClassRecognizer::createComparators() {
	int minOffset = 1-learnedData.size();
	int maxOffset = recognizingData.size()-1;

	std::cout << "Creating MotionComparators for " << this->motionClassName << "." << std::endl;
	for (int offset = minOffset; offset <= maxOffset; offset++) {
		std::cout << "Created " << offset-minOffset << "/" << maxOffset-minOffset << '\xd';
		this->comparators.push(MotionComparator(offset, learnedData, recognizingData));
	}
	std::cout << std::endl;
}

MotionComparator& MotionClassRecognizer::getBestComparator() {
	return comparators.top();
}

std::string MotionClassRecognizer::getClassName() {
	return this->motionClassName;
}

bool MotionClassRecognizer::compareFrame(unsigned int frame) {
	if (frame < 0 || frame >= this->learnedData.size()) {
		return false;
	}

	auto comparator = this->comparators.top();
	this->comparators.pop();

	comparator.compareFrame(frame);

	this->comparators.push(comparator);

	return true;
}