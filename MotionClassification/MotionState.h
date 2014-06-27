
#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H

#include "JoinBaseData.h"

class MotionState : public JoinBaseData
{
public:
	MotionState(std::string name);
	~MotionState();
	
private:
};

MotionState::MotionState(std::string name) : JoinBaseData(name) {
}

MotionState::~MotionState() {
}

#endif MOTIONSTATE_H