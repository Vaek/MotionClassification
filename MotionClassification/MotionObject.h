
#ifndef MOTIONOBJECT_H
#define MOTIONOBJECT_H

#include <vector>
#include "MotionFrame.h"

class MotionObject : public std::vector<MotionFrame>
{
public:
	MotionObject();
	~MotionObject();

	void setNodeImportance(std::string name, double importance);
	double getNodeImportance(std::string name);

private:
	std::map<std::string, double> nodeImportance;
};

#endif //!MOTIONOBJECT_H