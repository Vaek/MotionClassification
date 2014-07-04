
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
	AnimationCurve* getMotionCurve(std::string nodeName);
	AnimationCurveMap getAllMotionCurves();

private:
	int motionLength;
	AnimationCurveMap curves;
};

#endif MOTION_H