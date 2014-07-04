
#include "MotionFrame.h"

MotionFrame::MotionFrame() {
}

MotionFrame::~MotionFrame() {
	for (MotionStateMap::iterator it = this->states.begin(); it != this->states.end(); ++it) {
        delete it->second;
    }
    this->states.clear();
}

MotionState* MotionFrame::addMotionState(MotionState* state) {
	std::pair<MotionStateMap::iterator, bool> ret = this->states.insert(std::pair<std::string, MotionState*>(state->getName(), state));
	if (ret.second==false) {
		return ret.first->second;
	} else {
		return nullptr;
	}
}

MotionState* MotionFrame::getMotionState(std::string name) {
	return this->states.at(name);
}