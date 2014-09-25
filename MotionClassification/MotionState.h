
#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H

#include "JoinBaseData.h"

class MotionState : public JoinBaseData
{
public:
	MotionState(std::string name): JoinBaseData(name) {};
	~MotionState() {};
	
	friend std::ostream& operator<<(std::ostream& out, const MotionState& state);

private:
};

#endif MOTIONSTATE_H
