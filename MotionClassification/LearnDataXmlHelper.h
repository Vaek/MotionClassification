
#ifndef LEARNDATAXMLHELPER_H
#define LEARNDATAXMLHELPER_H

#include <vector>
#include "XmlHelper.h"
#include "MotionFrame.h"
#include "LearnDataContainer.h"

class LearnDataXmlHelper : public XmlHelper {
public:
	LearnDataXmlHelper();
	~LearnDataXmlHelper();

	void createDocument(std::map<std::string, MotionObject> data);
	std::map<std::string, MotionObject> readDocument();

private:
	bool createMotionObjectFile(std::string filePath, MotionObject motionObject);
	MotionObject loadMotionObject(std::string name, int lenght, std::string filePath);
};

#endif //!LEARNDATAXMLHELPER_H