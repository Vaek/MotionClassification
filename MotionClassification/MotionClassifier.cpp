
#include "MotionClassifier.h"

MotionClassifier::MotionClassifier()
{
	this->root = new MotionStateNode();
	this->root->classification = "Unknown Motion";
}

MotionClassifier::~MotionClassifier()
{
	delete root;
	root = nullptr;
}

void MotionClassifier::learnMotion(std::vector<MotionFrame> keyFrames, std::string classification) {

}