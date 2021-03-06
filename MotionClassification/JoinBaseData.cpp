
#include "JoinBaseData.h"

JoinBaseData::JoinBaseData(std::string name) : name(name) {
}

JoinBaseData::~JoinBaseData() {
}

JoinBaseData* JoinBaseData::setTranslation(std::array<double,3> translation) {
	this->translation = translation;
	return this;
}

JoinBaseData* JoinBaseData::setRotation(std::array<double,3> rotation) {
	this->rotation = rotation;
	return this;
}

JoinBaseData* JoinBaseData::setScaling(std::array<double,3> scaling) {
	this->scaling = scaling;
	return this;
}

std::string JoinBaseData::getName() const {
	return this->name;
}

const std::array<double,3> JoinBaseData::getTranslation() const {
	return this->translation;
}

const std::array<double,3> JoinBaseData::getRotation() const {
	return this->rotation;
}

const std::array<double,3> JoinBaseData::getScaling() const {
	return this->scaling;
}

double JoinBaseData::getSize(std::array<double, 3> arr3d) {
	double size = 0;

	for (int i = 0; i < 3; i++) {
		size = std::pow(arr3d[i], 2);
	}

	return std::sqrt(size);
}