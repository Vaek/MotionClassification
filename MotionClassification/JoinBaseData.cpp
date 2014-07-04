
#include "JoinBaseData.h"

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