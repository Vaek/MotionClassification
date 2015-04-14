
#include "MotionComparator.h"

MotionComparator::MotionComparator(const int startOffset, const std::vector<MotionFrame>& learnedData, const std::vector<MotionFrame>& recognizingData)
	: startOffset(startOffset), learnedData(learnedData), recognizingData(recognizingData), comparedFrames(0), similarity(1.0) {
}

MotionComparator::MotionComparator(const MotionComparator& comparator)
	: startOffset(comparator.startOffset), learnedData(comparator.learnedData), recognizingData(comparator.recognizingData), 
	comparedFrames(comparator.comparedFrames), similarity(comparator.similarity) {
}

MotionComparator::~MotionComparator() {}

double MotionComparator::stateDifference(MotionState learnedState, MotionState recognizingState) {
	double diffVectorSize = 0, learnedVectorSize = 0;

	for (int i = 0; i < 3; i++){
		diffVectorSize += std::pow(learnedState.getTranslation()[i] - recognizingState.getTranslation()[i], 2);
		learnedVectorSize += std::pow(learnedState.getTranslation()[i], 2);
	}

	diffVectorSize = std::sqrt(diffVectorSize);
	learnedVectorSize = std::sqrt(learnedVectorSize);

	if (learnedVectorSize > 0) {
		return std::min(diffVectorSize/learnedVectorSize, 1.0);
	} else {
		return 1.0;
	}
}

void MotionComparator::compareFrame(int frame) {
	comparedFrames++;
	auto comparingFrame = frame+this->startOffset;
	double frameSimilarity = 1.0;
	if (comparingFrame >= 0 && comparingFrame < this->recognizingData.size()) {
		auto learned = this->learnedData.at(frame);
		auto recognize = this->recognizingData.at(comparingFrame);
		
		double distance = 0.0;
		int numCompares = 0;
		for (auto learnedPair : learned.getAllStates()) {
			auto learnedState = learnedPair.second;
			if (recognize.hasMotionState(learnedState.getName())) {
				auto recogState = recognize.getMotionState(learnedState.getName());

				distance += stateDifference(learnedState, recogState);
				numCompares++;
			}
		}
		frameSimilarity =  1.0-(distance/numCompares);
	}
	similarity += frameSimilarity;
}

const double MotionComparator::getSimilarity() const {
	if (comparedFrames > 0) {
		return similarity/comparedFrames;
	} else {
		return 1.0;
	}
}

bool MotionComparator::operator<(const MotionComparator& rhs) const {
	return this->getSimilarity() < rhs.getSimilarity();
}

MotionComparator& MotionComparator::operator=(const MotionComparator& comparator) {
	MotionComparator tmp(comparator);
	return *this;
}
