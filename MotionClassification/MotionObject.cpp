
#include "MotionObject.h"

MotionObject::MotionObject() {
}

MotionObject::~MotionObject() {
	nodeImportance.clear();
}

void MotionObject::setNodeImportance(std::string name, double importance) {
	nodeImportance.insert(std::pair<std::string , double>(name, importance));
}

double MotionObject::getNodeImportance(std::string name) {
	return nodeImportance.at(name);
}
