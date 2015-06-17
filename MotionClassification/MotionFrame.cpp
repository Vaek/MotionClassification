
#include "MotionFrame.h"
#include <exception>

MotionFrame::MotionFrame() {
}

MotionFrame::~MotionFrame() {
    this->states.clear();
}

void MotionFrame::addMotionState(const MotionState state) {
	std::pair<MotionStateMap::iterator, bool> ret = this->states.insert(std::pair<std::string, MotionState>(state.getName(), state));
	/*
	if (ret.second==false) {
		std::clog << "State " << state << " already added.\n";
	}
	*/
}

MotionState MotionFrame::getMotionState(std::string name) {
	try {
		return this->states.at(name);
	} catch (std::exception e) {
		throw 0;
	}
}

bool MotionFrame::hasMotionState(std::string name) {
	auto it = this->states.find(name);
	if(it != this->states.end()) {
		return true;
	} else {
		return false;
	}
}

const MotionStateMap MotionFrame::getAllStates() {
	return this->states;
}

MotionFrame MotionFrame::averageWithFrame(MotionFrame& frameB) {
	MotionFrame averageFrame;
	for (auto statePair: this->getAllStates()) {
		MotionState state = statePair.second;
		if (frameB.hasMotionState(state.getName())) {
			auto motionState= frameB.getMotionState(state.getName());
			auto averageState = state.averageWithState(motionState);
			averageFrame.addMotionState(averageState);
		}
		averageFrame.addMotionState(MotionState(state));
	}
	return averageFrame;
}

std::ostream& operator<<(std::ostream& out, MotionFrame& frame) {
	out << "{motion : [";
	for (auto it = frame.getAllStates().begin(); it != frame.getAllStates().end(); ++it) {
        out << it->second << ",\n";
    }
	out << "]\n}";
	return out;
}
