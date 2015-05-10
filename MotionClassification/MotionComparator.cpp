
#include "MotionComparator.h"

MotionComparator::MotionComparator(const int startOffset, const MotionObject& learnedData, const MotionObject& recognizingData)
	: startOffset(startOffset), learnedData(learnedData), recognizingData(recognizingData), comparedFrames(0), difference(0.0), lastFrame(-1) {
}

MotionComparator::MotionComparator(const MotionComparator& comparator)
	: startOffset(comparator.startOffset), learnedData(comparator.learnedData), recognizingData(comparator.recognizingData), 
	comparedFrames(comparator.comparedFrames), difference(comparator.difference), lastFrame(comparator.lastFrame) {
}

MotionComparator::~MotionComparator() {}

double compareVector(std::array<double, 3> originVector, std::array<double, 3> toCompareVector) {
	double diffVectorSize = 0, learnedVectorSize = 0;

	for (int i = 0; i < 3; i++){
		diffVectorSize += std::pow(originVector[i] - toCompareVector[i], 2);
		learnedVectorSize += std::pow(originVector[i], 2);
	}

	diffVectorSize = std::sqrt(diffVectorSize);
	learnedVectorSize = std::sqrt(learnedVectorSize);

	if (learnedVectorSize > 0) {
		return std::min(diffVectorSize / learnedVectorSize, 1.0);
	} else {
		return 1.0;
	}
}

double MotionComparator::stateDifference(MotionState learnedState, MotionState recognizingState) {
	/* translation
	return compareVector(learnedState.getTranslation(), recognizingState.getTranslation());
	*/
	/*rotation*/
	return compareVector(learnedState.getRotation(), recognizingState.getRotation());
}

bool MotionComparator::compareNextFrame() {
	if (lastFrame + 1 >= this->learnedData.size()) return false;
	lastFrame++;
	return compareFrame(lastFrame);
}

bool MotionComparator::compareFrame(int frame) {
	comparedFrames++;
	this->lastFrame = frame;
	auto comparingFrame = frame+this->startOffset;
	double frameDifference = 1.0;
	if (comparingFrame >= 0 && comparingFrame < this->recognizingData.size()) {
		auto learned = this->learnedData.at(frame);
		auto recognize = this->recognizingData.at(comparingFrame);
		
		double distance = 0.0;
		int numCompares = 0;
		for (auto learnedPair : learned.getAllStates()) {
			auto learnedState = learnedPair.second;
			if (recognize.hasMotionState(learnedState.getName())) {
				auto recogState = recognize.getMotionState(learnedState.getName());
				double importance = this->learnedData.getNodeImportance(learnedState.getName());
				distance += stateDifference(learnedState, recogState) *(1.0 - importance);
				numCompares++;
			}
		}
		frameDifference = distance/numCompares;

		difference += frameDifference;
		return true;
	} else {
		difference += frameDifference;
		return comparingFrame >= this->recognizingData.size();
	}
}

std::pair<long, long> MotionComparator::getRange() {
	return std::pair<long, long>(startOffset, startOffset + this->recognizingData.size());
}

const double MotionComparator::getSimilarity() const {
	if (comparedFrames > 0) {
		double similarity = (((double)comparedFrames) - difference) / ((double)comparedFrames);
		return similarity;
	} else {
		return 0.0;
	}
}

const double MotionComparator::getCumulateDifference() const {
	return difference;
}

/*
used to sort comparators in priority queue, the bes comparator need to be first
*/
bool MotionComparator::operator<(const MotionComparator& rhs) const {
	return this->difference > rhs.difference;
}

bool MotionComparator::operator==(const MotionComparator& rhs) const {
	return this->startOffset == rhs.startOffset;
}

MotionComparator& MotionComparator::operator=(const MotionComparator& comparator) {
//	MotionComparator tmp(comparator);
	startOffset = comparator.startOffset;
	learnedData = comparator.learnedData;
	recognizingData = comparator.recognizingData;
	comparedFrames = comparator.comparedFrames;
	difference = comparator.difference;
	lastFrame = comparator.lastFrame;
	return *this;
}
