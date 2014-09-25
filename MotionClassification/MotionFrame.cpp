
#include "MotionFrame.h"
#include <exception>

MotionFrame::MotionFrame() {
}

MotionFrame::~MotionFrame() {
    this->states.clear();
}

void MotionFrame::addMotionState(const MotionState state) {
	std::pair<MotionStateMap::iterator, bool> ret = this->states.insert(std::pair<std::string, MotionState>(state.getName(), state));
	if (ret.second==false) {
		std::cout << "State " << state << " already added.\n";
	}
}

MotionState MotionFrame::getMotionState(std::string name) {
	try {
		return this->states.at(name);
	} catch (std::exception e) {
		throw 0;
	}
}

const MotionStateMap MotionFrame::getAllStates() {
	return this->states;
}

std::ostream& operator<<(std::ostream& out, MotionFrame& frame) {
	out << "{motion : [";
	for (auto it = frame.getAllStates().begin(); it != frame.getAllStates().end(); ++it) {
        out << it->second << ",\n";
    }
	out << "]\n}";
	return out;
}