
#ifndef MOTION_CLASS_RECOGNIZER
#define MOTION_CLASS_RECOGNIZER

#include <string>
#include <vector>
#include <queue>
#include "MotionFrame.h"
#include "MotionComparator.h"

class MotionClassRecognizer {
public:
	MotionClassRecognizer(const std::string motionClassName, const std::vector<MotionFrame> learnedData, const std::vector<MotionFrame> recognizingData);
	~MotionClassRecognizer();

	bool compareFrame(unsigned int frame);

	std::string getClassName();
	MotionComparator& getBestComparator();

private:
	void createComparators();

	const std::string motionClassName;
	const std::vector<MotionFrame> learnedData;
	const std::vector<MotionFrame> recognizingData;

	std::priority_queue<MotionComparator> comparators;
};

#endif // !MOTION_CLASS_RECOGNIZER

