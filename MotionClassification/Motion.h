
#ifndef MOTION_H
#define MOTION_H

#include <vector>
#include <map>
#include "MotionFrame.h"
#include "AnimationCurve.h"

typedef std::map<std::string, AnimationCurve*> AnimationCurveMap;

/**
 * Motion class contains whole data about skeletonNode transformation
 */
class Motion {
public:
	Motion();
	~Motion();

	int getMotionLength();
	
	MotionFrame* getFrame(int frame);
	AnimationCurve* addMotionCurve(std::string nodeName, AnimationCurve* curve);

private:
	int motionLength;
	AnimationCurveMap curves;
};

Motion::Motion() {
	this->motionLength = 0;
}

Motion::~Motion() {
	for (AnimationCurveMap::iterator it = this->curves.begin(); it != this->curves.end(); ++it) {
        delete it->second;
    }
	this->curves.clear();
}

int Motion::getMotionLength() {
	return this->motionLength;
}

MotionFrame* Motion::getFrame(int frame) {
	return NULL;
}

AnimationCurve* Motion::addMotionCurve(std::string nodeName, AnimationCurve* curve) {
	std::pair<AnimationCurveMap::iterator, bool> ret = this->curves.insert(std::pair<std::string, AnimationCurve*>(nodeName, curve));
	if (ret.second==false) {
		return ret.first->second;
	} else {
		return NULL;
	}
}

#endif MOTION_H