
#include "LearnDataXmlHelper.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>

char* NODE_MOTION_CLASS = "motion_class";
char* ATTR_REPEAT = "repeat";
char* ATTR_ORDERED = "ordered";

char* NODE_MOTION_OBJECT = "motion_object";
char* ATTR_LENGTH = "length";
char* ATTR_NAME2 = "name";
char* ATTR_PATH2 = "path";

const std::string LEARNED_DATA_FOLDER = "./learned/";

LearnDataXmlHelper::LearnDataXmlHelper() {}

LearnDataXmlHelper::~LearnDataXmlHelper() {}

void LearnDataXmlHelper::createDocument(std::map<std::string, MotionObject> data) {

	auto doctype = mXmlDocument.append_child(pugi::node_doctype);
	doctype.set_value("motion_class SYSTEM \"learned_data.dtd\"");

	auto nodeScene = mXmlDocument.append_child(NODE_MOTION_CLASS);
//	nodeScene.append_attribute(ATTR_REPEAT) = true;
//	nodeScene.append_attribute(ATTR_ORDERED) = true;

	for (auto learned : data) {
		std::string filePath = learned.first;
		if (createMotionObjectFile(LEARNED_DATA_FOLDER + filePath, learned.second)) {
			auto child = nodeScene.append_child(NODE_MOTION_OBJECT);
			child.append_attribute(ATTR_NAME2) = learned.first.c_str();
			child.append_attribute(ATTR_LENGTH) = learned.second.size();
			child.append_attribute(ATTR_PATH2) = filePath.c_str();
		}
	}
}

const std::string IMPORTANCES = "importances";
const std::string OFFSETS = "offsets";
const std::string FRAME = "frame";

bool LearnDataXmlHelper::createMotionObjectFile(std::string filePath, MotionObject motionObject) {

	std::ofstream myfile(filePath, std::ios::beg);
	if (myfile.is_open()) {
		myfile << IMPORTANCES << " " << motionObject.getNodeImportances().size() << std::endl;
		for each (auto importancePair in motionObject.getNodeImportances()) {
			myfile << importancePair.first << " : " << importancePair.second << std::endl;
		}
		myfile << OFFSETS << " " << motionObject.getNodeOffsets().size() << std::endl;
		for each (auto offsetPair in motionObject.getNodeOffsets()) {
			myfile << offsetPair.first << " : " << offsetPair.second << std::endl;
		}
		for (auto i = 0; i < motionObject.size(); i++) {
			auto states = motionObject.at(i).getAllStates();
			myfile << FRAME << " " << i << " " << states.size() << std::endl;
			for (auto state : states) {
				auto rotation = state.second.getRotation();
				auto translation = state.second.getTranslation();
				auto scaling = state.second.getScaling();
				auto name = state.second.getName();
				std::replace(name.begin(), name.end(), ' ', '_');
				myfile << name << " : " << rotation[0] << " " << rotation[1] << " " << rotation[2]
												 << " " << translation[0] << " " << translation[1] << " " << translation[2] 
												 << " " << scaling[0] << " " << scaling[1] << " " << scaling[2] << std::endl;
			}
		}
		myfile.close();
		return true;
	}
	
	return false;
}

std::map<std::string, MotionObject> LearnDataXmlHelper::readDocument() {
	std::map<std::string, MotionObject> data;
	std::stack<pugi::xml_node> stack;

	stack.push(mXmlDocument.child(NODE_MOTION_CLASS));
	while (!stack.empty()) {
		auto node = stack.top();
		stack.pop();

		for (pugi::xml_node child = node.child(NODE_MOTION_CLASS); child; child = child.next_sibling(NODE_MOTION_CLASS)) {
			stack.push(child);
		}
		for (pugi::xml_node child = node.child(NODE_MOTION_OBJECT); child; child = child.next_sibling(NODE_MOTION_OBJECT)) {
			try {
				std::string name = child.attribute(ATTR_NAME2).value();
				int length = std::stoi(child.attribute(ATTR_LENGTH).value());
				std::string path = LEARNED_DATA_FOLDER + child.attribute(ATTR_PATH2).value();
				data.insert(std::pair<std::string, MotionObject>(name, loadMotionObject(name, length, path)));
				std::cout << "Loaded motion object " << name << std::endl;
			} catch (std::string e) {
				std::cout << e << std::endl;
			}
		}
	}
	return data;
}

void readImportances(std::ifstream& file, MotionObject& object, int nodes) {
	std::string line;
	for (auto i = 0; i < nodes; i++) {
		std::getline(file, line);
		std::vector<std::string> tokens{ std::istream_iterator < std::string > {std::istringstream(line)}, std::istream_iterator < std::string > {} };
		object.setNodeImportance(tokens.at(0), std::stod(tokens.at(2)));
	}
}

void readOffsets(std::ifstream& file, MotionObject& object, int nodes) {
	std::string line;
	for (auto i = 0; i < nodes; i++) {
		std::getline(file, line);
		std::vector<std::string> tokens{ std::istream_iterator < std::string > {std::istringstream(line)}, std::istream_iterator < std::string > {} };
		object.setNodeOffset(std::pair<std::string, double>(tokens.at(0), std::stod(tokens.at(2))));
	}
}

void readFrame(std::ifstream& file, MotionFrame& frame, int nodes) {
	std::string line;
	for (auto i = 0; i < nodes; i++) {
		std::getline(file, line);
		std::vector<std::string> tokens{ std::istream_iterator < std::string > {std::istringstream(line)}, std::istream_iterator < std::string > {} };

		MotionState state(tokens.at(0));
		std::array<double, 3> rotation = { std::stod(tokens.at(2)), std::stod(tokens.at(3)), std::stod(tokens.at(4)) };
		std::array<double, 3> translation = { std::stod(tokens.at(5)), std::stod(tokens.at(6)), std::stod(tokens.at(7)) };
		std::array<double, 3> scaling = { std::stod(tokens.at(8)), std::stod(tokens.at(9)), std::stod(tokens.at(10)) };
		state.setRotation(rotation);
		state.setScaling(scaling);
		state.setTranslation(translation);
		frame.addMotionState(state);
	}
}

MotionObject LearnDataXmlHelper::loadMotionObject(std::string name, int lenght, std::string filePath) {
	MotionObject mo;
	std::ifstream file(filePath);
	
	if (file.is_open()) {
		std::string line;
		int items;
		MotionFrame frame;
		std::vector<std::string> tokens;
		int reading = 0;
		while (std::getline(file, line)) {
			std::vector<std::string> tokens{std::istream_iterator<std::string>{std::istringstream(line)}, std::istream_iterator<std::string>{}};
//			copyTokens(line, tokens);
			
			if (!tokens.empty()) {
				if (tokens.at(0) == IMPORTANCES) {
					readImportances(file, mo, std::stoi(tokens.at(1)));
				} else if (tokens.at(0) == OFFSETS) {
					readOffsets(file, mo, std::stoi(tokens.at(1)));
				} else if (tokens.at(0) == FRAME) {// << "frame " << i << std::endl;
//					if (std::stoi(tokens.at(1)) > 0) mo.push_back(frame);
					frame = MotionFrame();
					readFrame(file, frame, std::stoi(tokens.at(2)));
					mo.push_back(frame);
				}
			}
		}
		file.close();
	} else {
		throw std::string("File "+filePath+" not found.");;
	}
	return mo;
}