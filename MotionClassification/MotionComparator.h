
#ifndef MOTION_COMPARATOR
#define MOTION_COMPARATOR

#include <vector>
#include "MotionFrame.h"
#include "MotionObject.h"

class MotionComparator {
public:
	MotionComparator(const int startOffset, const MotionObject& learnedData, const MotionObject& recognizingData);
	MotionComparator(const MotionComparator& comparator);
	~MotionComparator();

	bool compareFrame(int frame);
	bool compareNextFrame();

	const double getCumulateDifference() const;
	const double getSimilarity() const;
	std::pair<long, long> getRange();
	bool operator<(const MotionComparator& rhs) const;
	bool operator==(const MotionComparator& rhs) const;
	MotionComparator& operator=(const MotionComparator& comparator);

	static double stateDifference(MotionState learnedState, MotionState recognizingState);
	
private:
	int lastFrame;
	int startOffset;
	MotionObject learnedData;
	MotionObject recognizingData;

	double difference;
	int comparedFrames;
};

#endif // !MOTION_COMPARATOR