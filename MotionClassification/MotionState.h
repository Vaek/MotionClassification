
#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H

#include "JoinBaseData.h"
#include "Vector.h"

class MotionState : public JoinBaseData
{
public:
	MotionState(std::string name): JoinBaseData(name) {};
	~MotionState() {};
	
	friend std::ostream& operator<<(std::ostream& out, const MotionState& state);
	MotionState averageWithState(MotionState& state);

private:
	std::array<double, 3> averageVector(std::array<double, 3> vector1, std::array<double, 3> vector2);
	std::array<double, 3> changeVectorSize(std::array<double, 3> vector, double size);
};

#endif MOTIONSTATE_H
