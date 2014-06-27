
#ifndef ANIMATIONCURVE_H
#define ANIMATIONCURVE_H

#include <vector>
#include <array>

class AnimationCurve
{
public:
	AnimationCurve(std::string name);
	~AnimationCurve();
	
	AnimationCurve* setTranslation(int frame, const double *translation);
	AnimationCurve* setRotation(int frame, const double *rotation);
	AnimationCurve* setScaling(int frame, const double *scaling);

	std::array<double,4> getTranslation(int frame);
	std::array<double,4> getRotation(int frame);
	std::array<double,4> getScaling(int frame);

	int getLength();

	std::string getName();

	void reserve(int frameNumber);

private:
	std::string name;
	std::vector<std::array<double,4>> translation, rotation, scaling;
};

AnimationCurve::AnimationCurve(std::string name) : name(name) {
}

AnimationCurve::~AnimationCurve() {
}

void AnimationCurve::reserve(int frameNumber) {
	//double nullArray[4] = {0.0, 0.0, 0.0, 0.0};
	this->translation.resize(frameNumber);
	this->rotation.resize(frameNumber);
	this->scaling.resize(frameNumber);
}

std::string AnimationCurve::getName() {
	return this->name;
}

AnimationCurve* AnimationCurve::setTranslation(int frame, const double *translation) {
	std::copy(translation, translation+4, this->translation[frame].data());
	return this;
}

AnimationCurve* AnimationCurve::setRotation(int frame, const double* rotation) {
//	this->rotation.resize(frame);
	std::copy(rotation, rotation+4, this->rotation[frame].data());
	return this;
}

AnimationCurve* AnimationCurve::setScaling(int frame, const double* scaling) {
//	this->scaling.resize(frame);
	std::copy(scaling, scaling+4, this->scaling[frame].data());
	return this;
}

std::array<double,4> AnimationCurve::getTranslation(int frame) {
	return this->translation[frame];
}

std::array<double,4> AnimationCurve::getRotation(int frame) {
	return this->rotation[frame];
}

std::array<double,4> AnimationCurve::getScaling(int frame) {
	return this->scaling[frame];
}

int AnimationCurve::getLength() {
	return this->translation.size();
}

std::ostream& operator<< (std::ostream& out, AnimationCurve& curve) {
	out << "{\"" << curve.getName() << "\": " << std::endl;
	for (int f = 0; f < curve.getLength(); f++) {
		auto t = curve.getTranslation(f);
		auto r = curve.getRotation(f);
		auto s = curve.getScaling(f);
		out << "translation : {" <<
				"x:" << t[0] << ", " << 
				"y:" << t[1] << ", " << 
				"z:" << t[2] << ", " << 
		   "},\n" <<
		   "rotation : {" << 
				"x:" << r[0] << ", " << 
				"y:" << r[1] << ", " << 
				"z:" << r[2] << ", " << 
		   "},\n" <<
		   "scaling : {" 
				"x:" << s[0] << ", " << 
				"y:" << s[1] << ", " << 
				"z:" << s[2] << ", " << 
		   "},\n";
	}

	return out;
}

#endif ANIMATIONCURVE_H