
#include "LearnDataXmlHelper.h"
#include <iostream>
#include <fstream>

char* NODE_MOTION_CLASS = "motion_class";
char* ATTR_REPEAT = "repeat";
char* ATTR_ORDERED = "ordered";

char* NODE_MOTION_OBJECT = "motion_object";
char* ATTR_LENGTH = "length";
char* ATTR_NAME = "name";
char* ATTR_PATH = "path";

LearnDataXmlHelper::LearnDataXmlHelper() {}

LearnDataXmlHelper::~LearnDataXmlHelper() {}

void LearnDataXmlHelper::createDocument(std::map<std::string, MotionObject> data) {

	auto doctype = mXmlDocument.append_child(pugi::node_doctype);
	doctype.set_value("scene SYSTEM \"learned_data.dtd\"");

	auto nodeScene = mXmlDocument.append_child(NODE_MOTION_CLASS);
//	nodeScene.append_attribute(ATTR_REPEAT) = true;
//	nodeScene.append_attribute(ATTR_ORDERED) = true;

	for (auto learned : data) {
		std::string filePath = learned.first;
		if (createMotionObjectFile(filePath, learned.second)) {
			auto child = nodeScene.append_child(NODE_MOTION_OBJECT);
			child.append_attribute(ATTR_NAME) = learned.first.c_str();
			child.append_attribute(ATTR_LENGTH) = learned.second.size();
			child.append_attribute(ATTR_PATH) = filePath.c_str();
		}
	}
}

bool LearnDataXmlHelper::createMotionObjectFile(std::string filePath, MotionObject motionObject) {

	std::ofstream myfile(filePath);
	if (myfile.is_open()) {
		for (size_t i = 0; i < motionObject.size(); i++) {
			myfile << "frame " << i << std::endl;
			auto frame = motionObject.at(i);
			for (auto state : frame.getAllStates()) {
				auto rotation = state.second.getRotation();
				auto translation = state.second.getTranslation();
				auto scaling = state.second.getScaling();
				myfile << state.second.getName() << " " << rotation[0] << " " << rotation[1] << " " << rotation[2]
												 << " " << translation[0] << " " << translation[1] << " " << translation[2] 
												 << " " << scaling[0] << " " << scaling[1] << " " << scaling[2] << std::endl;
			}
		}
		myfile.close();
		return true;
	}
	
	return false;
}