
#ifndef ANIMATIONCURVE_H
#define ANIMATIONCURVE_H

#include <vector>
#include <array>

class AnimationCurve
{
public:
	AnimationCurve(std::string name);
	~AnimationCurve();
	
	AnimationCurve* setTranslation(int frame, double *translation);
	AnimationCurve* setRotation(int frame, double rotation[4]);
	AnimationCurve* setScaling(int frame, double scaling[4]);

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
}

std::string AnimationCurve::getName() {
	return this->name;
}

AnimationCurve* AnimationCurve::setTranslation(int frame, double *translation) {
//	this->translation.resize(frame);
	std::copy(&translation[0], &translation[4], this->translation[frame].data());
	return this;
}

AnimationCurve* AnimationCurve::setRotation(int frame, double rotation[4]) {
//	this->rotation.resize(frame);
	std::copy(rotation, rotation+4, this->rotation[frame].data());
	return this;
}

AnimationCurve* AnimationCurve::setScaling(int frame, double scaling[4]) {
//	this->scaling.resize(frame);
	std::copy(scaling, scaling+4, this->scaling[frame].data());
	return this;
}


#endif ANIMATIONCURVE_H