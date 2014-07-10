
#include "AnimationCurve.h"
#include <iostream>
#include <string>
#include <iomanip>

AnimationCurve::AnimationCurve(std::string name) : name(name) {
}

AnimationCurve::~AnimationCurve() {
}

void AnimationCurve::reserve(int frameNumber) {
	//double nullArray[3] = {0.0, 0.0, 0.0};
	this->translation.resize(frameNumber);
	this->rotation.resize(frameNumber);
	this->scaling.resize(frameNumber);
}

std::string AnimationCurve::getName() {
	return this->name;
}

AnimationCurve* AnimationCurve::setTranslation(int frame, const std::array<double,3> translation) {
	this->translation[frame] = translation;
	return this;
}

AnimationCurve* AnimationCurve::setRotation(int frame, const std::array<double,3> rotation) {
	this->rotation[frame] = rotation;
	return this;
}

AnimationCurve* AnimationCurve::setScaling(int frame, const std::array<double,3> scaling) {
	this->scaling[frame] = scaling;
	return this;
}

std::array<double,3> AnimationCurve::getTranslation(int frame) {
	return this->translation[frame];
}

std::array<double,3> AnimationCurve::getRotation(int frame) {
	return this->rotation[frame];
}

std::array<double,3> AnimationCurve::getScaling(int frame) {
	return this->scaling[frame];
}

int AnimationCurve::getLength() {
	return this->translation.size();
}

std::ostream& operator<< (std::ostream& out, AnimationCurve& curve) {
	out << "{\"" << curve.getName() << "\": " << std::endl;
	int w = 15;
	for (int f = 0; f < curve.getLength(); f++) {
		auto t = curve.getTranslation(f);
		auto r = curve.getRotation(f);
		auto s = curve.getScaling(f);
		out << "translation:\t{" <<
				"x:" << std::right << std::setw(w) << std::setfill(' ') << t[0] << ", " << 
				"y:" << std::right << std::setw(w) << std::setfill(' ') << t[1] << ", " << 
				"z:" << std::right << std::setw(w) << std::setfill(' ') << t[2] << ", " << 
		   "}," <<
		   "rotation:\t{" << 
				"x:" << std::right << std::setw(w) << std::setfill(' ') << r[0] << ", " << 
				"y:" << std::right << std::setw(w) << std::setfill(' ') << r[1] << ", " << 
				"z:" << std::right << std::setw(w) << std::setfill(' ') << r[2] << ", " << 
		   "}," <<
		   "scaling:\t{" 
				"x:" << std::right << std::setw(w) << std::setfill(' ') << s[0] << ", " << 
				"y:" << std::right << std::setw(w) << std::setfill(' ') << s[1] << ", " << 
				"z:" << std::right << std::setw(w) << std::setfill(' ') << s[2] << ", " << 
		   "}\n";
	}

	out << "}";
	return out;
}
