
#ifndef ANIMATIONCURVE_H
#define ANIMATIONCURVE_H

#include <vector>

class AnimationCurve
{
public:
	AnimationCurve(std::string name);
	~AnimationCurve();
	
	AnimationCurve* setTranslation(int frame, double translation[4]);
	AnimationCurve* setRotation(int frame, double rotation[4]);
	AnimationCurve* setScaling(int frame, double scaling[4]);

private:
	std::string name;
	std::vector<double[4]> translation, rotation, scaling;
};

AnimationCurve::AnimationCurve(std::string name) : name(name) {
}

AnimationCurve::~AnimationCurve() {
}

AnimationCurve* AnimationCurve::setTranslation(int frame, double translation[4]) {
	std::copy(translation, translation+4, this->translation[frame]);
	return this;
}

AnimationCurve* AnimationCurve::setRotation(int frame, double rotation[4]) {
	std::copy(rotation, rotation+4, this->rotation[frame]);
	return this;
}

AnimationCurve* AnimationCurve::setScaling(int frame, double scaling[4]) {
	std::copy(scaling, scaling+4, this->scaling[frame]);
	return this;
}


#endif ANIMATIONCURVE_H