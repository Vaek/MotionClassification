
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

std::ostream& operator<< (std::ostream& out, AnimationCurve& curve);

#endif ANIMATIONCURVE_H
