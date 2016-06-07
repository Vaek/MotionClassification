
#ifndef LEARNDATACONTAINER_H
#define LEARNDATACONTAINER_H

#include <map>
#include <string>
#include "MotionFrame.h"
#include "MotionObject.h"
#include "XmlHelper.h"
#include "MotionClassRecognizer.h"

class LearnDataContainer {
public:
	LearnDataContainer();
	LearnDataContainer(const std::string fileName);
	~LearnDataContainer();

	void updateLearnMotion(std::string motionClass, MotionObject motionObject);
	void combineAndUpdateLearnMotion(std::string motionClass, std::vector<MotionObject> keyframes);
	MotionObject getLearnMotionObject(std::string motionClass);
//	std::string recognizeMotionClass(MotionObject motionObject);
	std::map<std::pair<long, long>, std::string> recognizeMotionClass(MotionObject motionObject);

	bool saveLearnedData();
	bool loadLearnedData();

	std::map<std::string, MotionObject> data;
private:
	const std::string fileName;

	std::vector<MotionClassRecognizer> proccesRecognizing(MotionObject motionObject);
};

#endif //!LEARNDATACONTAINER_H
