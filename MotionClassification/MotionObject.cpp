
#include "MotionObject.h"

const double MotionObject::IMPORTANCE_LIMIT = 0.2;

MotionObject::MotionObject() {
}

MotionObject::~MotionObject() {
	nodeImportances.clear();
	nodeOffsets.clear();
}

void MotionObject::setNodeImportance(std::string name, double importance) {
	nodeImportances.insert(std::pair<std::string , double>(name, importance));
}

double MotionObject::getNodeImportance(std::string name) {
	return nodeImportances.at(name);
}
std::map<std::string, double> MotionObject::getNodeImportances() {
	return nodeImportances;
}

void MotionObject::setNodeOffset(std::pair<std::string, double> offset) {
	nodeOffsets.insert(offset);
}

double MotionObject::getNodeOffset(std::string name) {
	return nodeOffsets.at(name);
}

std::map<std::string, double> MotionObject::getNodeOffsets() {
	return nodeOffsets;
}