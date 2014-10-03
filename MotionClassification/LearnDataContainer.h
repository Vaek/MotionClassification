
#ifndef LEARNDATACONTAINER_H
#define LEARNDATACONTAINER_H

#include <map>
#include "MotionFrame.h"

class LearnDataContainer {
public:
	LearnDataContainer();
	~LearnDataContainer();

	void updateLearnMotion(std::string motionClass, std::vector<MotionFrame> keyFrames);
	std::vector<MotionFrame> getLearnMotionObject(std::string motionClass);
	std::string recognizeMotionClass(std::vector<MotionFrame> keyFrames);

private:
	std::map<std::string, std::vector<MotionFrame>> data;
	double compareMotionObjects(std::vector<MotionFrame> learned, std::vector<MotionFrame> recognize);
	double compareFrames(MotionFrame learned, MotionFrame recognize);
};

#endif LEARNDATACONTAINER_H