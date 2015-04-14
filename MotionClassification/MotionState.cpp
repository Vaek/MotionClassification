
#include "MotionState.h"
#include <string>

std::array<double, 3> MotionState::averageVector(std::array<double, 3> vector1, std::array<double, 3> vector2) {
	std::array<double, 3> averageVector;
	for (int i = 0; i < 3; i++) {
		averageVector[i] = (vector1[i] + vector2[i]) / 2.0;
	}
	return averageVector;
}

std::array<double, 3> MotionState::changeVectorSize(std::array<double, 3> vector, double size) {
	auto originSize = getSize(vector);
	for (int i = 0; i < 3; i++) {
		vector[i] = vector[i] / originSize * size;
	}
	return vector;
}

MotionState MotionState::averageWithState(MotionState& state) {
	MotionState averageState(getName());
	averageState.setRotation(averageVector(getRotation(), state.getRotation()));
	auto averageSize = (getSize(getTranslation()) + getSize(state.getTranslation()))/2;
	averageState.setTranslation(changeVectorSize(averageVector(getTranslation(), state.getTranslation()), averageSize));
	averageState.setScaling(averageVector(getScaling(), state.getScaling()));
	return averageState;
}

std::ostream& operator<<(std::ostream& out, const MotionState& state) {
	out << "{name : " << state.getName() << ", \n"
		<< "rotation : {\n"
			<<"x : " << state.getRotation()[0] << ", \n"
			<<"y : " << state.getRotation()[1] << ", \n"
			<<"z : " << state.getRotation()[2] << "}, \n"
		<< "translation : {\n"
			<<"x : " << state.getTranslation()[0] << ", \n"
			<<"y : " << state.getTranslation()[1] << ", \n"
			<<"z : " << state.getTranslation()[2] << "}, \n"
		<< "scaling : {\n"
			<<"x : " << state.getScaling()[0] << ", \n"
			<<"y : " << state.getScaling()[1] << ", \n"
			<<"z : " << state.getScaling()[2] << "}} \n";
	return out;
}