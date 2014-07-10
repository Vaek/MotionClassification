
#include "Motion.h"

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

AnimationCurve* Motion::addAnimationCurve(std::string nodeName, AnimationCurve* curve) {
	std::pair<AnimationCurveMap::iterator, bool> ret = this->curves.insert(std::pair<std::string, AnimationCurve*>(nodeName, curve));
	if (ret.second==false) {
		return ret.first->second;
	} else {
		return NULL;
	}
}

AnimationCurve* Motion::getAnimationCurve(std::string nodeName) {
	return this->curves.at(nodeName);
}

AnimationCurveMap Motion::getAllAnimationCurves() {
	return this->curves;
}

std::ostream& operator<<(std::ostream& out, Motion& motion) {
	AnimationCurveMap curves = motion.getAllAnimationCurves();
	out << "{motion : [";
	for (AnimationCurveMap::iterator it = curves.begin(); it != curves.end(); ++it) {
        out << *(it->second) << ",\n";
    }
	out << "]\n}";
	return out;
}