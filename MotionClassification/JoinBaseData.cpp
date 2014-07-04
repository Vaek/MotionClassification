
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

std::string JoinBaseData::getName() const {
	return this->name;
}

const double *JoinBaseData::getTranslation() const {
	return this->translation;
}

const double* JoinBaseData::getRotation() const {
	return this->rotation;
}

const double* JoinBaseData::getScaling() const {
	return this->scaling;
}
