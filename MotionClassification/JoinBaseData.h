
#ifndef JOINBASEDATA_H
#define JOINBASEDATA_H

#include <iostream>

class JoinBaseData {
public:
	JoinBaseData(std::string name);
	virtual ~JoinBaseData();
	
	JoinBaseData* setTranslation(double* translation);
	JoinBaseData* setRotation(double* rotation);
	JoinBaseData* setScaling(double* scaling);
	
	const std::string getName();
	double* getTranslation();
	double* getRotation();
	double* getScaling();

private:
	const std::string name;
	double translation[3], rotation[3], scaling[3];
};

JoinBaseData::JoinBaseData(std::string name) : name(name) {
}

JoinBaseData::~JoinBaseData() {
}

JoinBaseData* JoinBaseData::setTranslation(double* translation) {
	std::copy(translation, translation+3, this->translation);
	return this;
}

JoinBaseData* JoinBaseData::setRotation(double* rotation) {
	std::copy(rotation, rotation+3, this->rotation);
	return this;
}

JoinBaseData* JoinBaseData::setScaling(double* scaling) {
	std::copy(scaling, scaling+3, this->scaling);
	return this;
}

const std::string JoinBaseData::getName() {
	return this->name;
}

double* JoinBaseData::getTranslation() {
	return this->translation;
}

double* JoinBaseData::getRotation() {
	return this->rotation;
}

double* JoinBaseData::getScaling() {
	return this->scaling;
}

#endif JOINBASEDATA