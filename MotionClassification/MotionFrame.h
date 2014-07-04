
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

#endif MOTIONFRAME_H