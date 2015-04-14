
#ifndef LEARNDATACONTAINER_H
#define LEARNDATACONTAINER_H

#include <map>
#include <string>
#include "MotionFrame.h"
#include "MotionObject.h"
#include "XmlHelper.h"

class LearnDataContainer {
public:
	LearnDataContainer();
	LearnDataContainer(const std::string fileName);
	~LearnDataContainer();

	void updateLearnMotion(std::string motionClass, MotionObject motionObject);
	void combineAndUpdateLearnMotion(std::string motionClass, std::vector<MotionObject> keyframes);
	MotionObject getLearnMotionObject(std::string motionClass);
	std::string recognizeMotionClass(MotionObject motionObject);

	bool saveLearnedData();
	bool loadLearnedData();

private:
	const std::string fileName;
	std::map<std::string, MotionObject> data;
};

#endif //!LEARNDATACONTAINER_H
