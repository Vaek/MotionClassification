
#include "MotionFrame.h"

MotionFrame::MotionFrame() {
}

MotionFrame::~MotionFrame() {
    this->states.clear();
}

void MotionFrame::addMotionState(const MotionState state) {
	std::pair<MotionStateMap::iterator, bool> ret = this->states.insert(std::pair<std::string, MotionState>(state.getName(), state));
	if (ret.second!=false) {
		std::cout << "State already added";
	}
}

MotionState MotionFrame::getMotionState(std::string name) {
	return this->states.at(name);
}

const MotionStateMap MotionFrame::getAllStates() {
	return this->states;
}