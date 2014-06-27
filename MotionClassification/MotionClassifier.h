
#ifndef MOTIONCLASSIFIER_H
#define MOTIONCLASSIFIER_H

#include <vector>
#include "KeyFrame.h"
#include "MotionStateNode.h"

class MotionClassifier
{
public:
	MotionClassifier();
	~MotionClassifier();

	MotionStateNode* root;

	void learnMotion(std::vector<KeyFrame> keyFrames, std::string classification);

private:

};

#endif //MOTIONCLASSIFIER_H