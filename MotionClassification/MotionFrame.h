
#ifndef MOTIONFRAME_H
#define MOTIONFRAME_H

#include <map>
#include "SkeletonNode.h"
#include "MotionState.h"

typedef std::map<std::string, MotionState*> MotionStateMap;

class MotionFrame {
public:
	MotionFrame();
	~MotionFrame();
	
	MotionState* addMotionState(MotionState* state);
	MotionState* getMotionState(std::string name);

private:
	MotionStateMap states;
};

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
		return NULL;
	}
}

MotionState* MotionFrame::getMotionState(std::string name) {
	return this->states.at(name);
}

#endif MOTIONFRAME_H