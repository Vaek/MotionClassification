
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
	std::map<std::string, double> getNodeImportances();
	void setNodeOffset(std::pair<std::string, double> offset);
	double getNodeOffset(std::string name);
	std::map<std::string, double> getNodeOffsets();

	static const double IMPORTANCE_LIMIT;

private:
	std::map<std::string, double> nodeImportances;
	std::map<std::string, double> nodeOffsets;
};

#endif //!MOTIONOBJECT_H