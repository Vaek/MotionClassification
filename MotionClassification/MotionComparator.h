
#ifndef MOTION_COMPARATOR
#define MOTION_COMPARATOR

#include <vector>
#include "MotionFrame.h"

class MotionComparator {
public:
	MotionComparator(const int startOffset, const std::vector<MotionFrame>& learnedData, const std::vector<MotionFrame>& recognizingData);
	MotionComparator(const MotionComparator& comparator);
	~MotionComparator();
	
	void compareFrame(int frame);

	const double getSimilarity() const;
	bool MotionComparator::operator<(const MotionComparator& rhs) const;
	MotionComparator& operator=(const MotionComparator& comparator);

	static double stateDifference(MotionState learnedState, MotionState recognizingState);

private:

	const int startOffset;
	const std::vector<MotionFrame>& learnedData;
	const std::vector<MotionFrame>& recognizingData;

	double similarity;
	int comparedFrames;
};

#endif // !MOTION_COMPARATOR