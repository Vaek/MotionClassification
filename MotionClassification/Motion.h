
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
	AnimationCurve* addAnimationCurve(std::string nodeName, AnimationCurve* curve);
	AnimationCurve* getAnimationCurve(std::string nodeName);
	AnimationCurveMap getAllAnimationCurves();

private:
	int motionLength;
	AnimationCurveMap curves;
};


std::ostream& operator<<(std::ostream& out, Motion& motion);

#endif MOTION_H
