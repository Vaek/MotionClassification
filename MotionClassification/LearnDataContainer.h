
#ifndef LEARNDATACONTAINER_H
#define LEARNDATACONTAINER_H

#include <map>
#include <string>
#include "MotionFrame.h"
#include "XmlHelper.h"

typedef std::vector<MotionFrame> MotionObject;

class LearnDataContainer {
public:
	LearnDataContainer();
	LearnDataContainer(const std::string fileName);
	~LearnDataContainer();

	void updateLearnMotion(std::string motionClass, MotionObject motionObject);
	MotionObject getLearnMotionObject(std::string motionClass);
	std::string recognizeMotionClass(MotionObject motionObject);

	bool saveLearnedData();

private:
	const std::string fileName;
	std::map<std::string, MotionObject> data;
};

#endif //!LEARNDATACONTAINER_H
