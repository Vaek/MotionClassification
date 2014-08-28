
#ifndef MOTIONCLASSIFIER_H
#define MOTIONCLASSIFIER_H

#include <vector>
#include "MotionStateNode.h"
#include "MotionFrame.h"

class MotionClassifier
{
public:
	MotionClassifier();
	~MotionClassifier();

	MotionStateNode* root;

	void learnMotion(std::vector<MotionFrame> keyFrames, std::string classification);

private:

};

#endif //MOTIONCLASSIFIER_H