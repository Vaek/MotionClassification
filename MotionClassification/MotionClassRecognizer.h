
#ifndef MOTION_CLASS_RECOGNIZER
#define MOTION_CLASS_RECOGNIZER

#include <string>
#include <vector>
#include <queue>
#include "MotionFrame.h"
#include "MotionComparator.h"

class DifferenceComparison {
	bool reverse;
public:
	DifferenceComparison(const bool& revparam = false) {
		reverse = revparam;
	}

	bool operator() (const MotionComparator& lhs, const MotionComparator&rhs) const {
		if (reverse) return (lhs.getCumulateDifference() > rhs.getCumulateDifference());
		else return (lhs<rhs);
	}
};

class MotionClassRecognizer {
public:
	MotionClassRecognizer(const std::string motionClassName, const MotionObject learnedData, const MotionObject recognizingData);
	~MotionClassRecognizer();

	bool compareFrames();

	std::string getClassName();
	std::vector<MotionComparator> getBestComparators();

	static const double MotionClassRecognizer::RECOGNIZE_LIMIT;

private:
	void createComparators();

	const std::string motionClassName;
	const MotionObject learnedData;
	const MotionObject recognizingData;

	std::vector<MotionComparator> bestComparators;
	std::priority_queue<MotionComparator> comparators;
};

#endif // !MOTION_CLASS_RECOGNIZER

