
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

std::array<double, 3> getNullTranformation() {
	std::array<double, 3> transformation = {0, 0, 0};
	return transformation;
}

MotionFrame Motion::getFrame(int frame) {
	MotionFrame motionFrame;
	for (auto it = curves.begin(); it != curves.end(); ++it) {
		auto curve = it->second;
		if (curve->getLength()>=0 && curve->getLength()>frame) {
			MotionState state(it->first);
			state.setTranslation(curve->getTranslation(frame));
			state.setRotation(curve->getRotation(frame));
			state.setScaling(curve->getScaling(frame));
			motionFrame.addMotionState(state);
		} else {
			MotionState state(it->first);
			state.setTranslation(getNullTranformation());
			state.setRotation(getNullTranformation());
			state.setScaling(getNullTranformation());
			motionFrame.addMotionState(state);
		}
    }
	return motionFrame;
}

AnimationCurve* Motion::addAnimationCurve(std::string nodeName, AnimationCurve* curve) {
	this->motionLength = std::max(this->motionLength, curve->getLength());
	auto ret = this->curves.insert(std::pair<std::string, AnimationCurve*>(nodeName, curve));
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