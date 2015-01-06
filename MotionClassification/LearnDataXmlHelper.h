
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

private:
	bool createMotionObjectFile(std::string filePath, MotionObject motionObject);
};

#endif //!LEARNDATAXMLHELPER_H