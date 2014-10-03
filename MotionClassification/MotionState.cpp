
#include "MotionState.h"
#include <string>

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