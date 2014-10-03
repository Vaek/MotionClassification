
#include "XmlHelper.h"
#include <fstream>
#include <stack>
#include <vector>
#include <iostream>

char* NODE_NODE = "node";
char* NODE_SCENE = "scene";
char* NODE_STRUCTURE = "structure";
char* NODE_ANNOTATION = "annotation";
char* ATTR_NAME = "name";
char* ATTR_PATH = "path";
char* ATTR_ANNOTATION = "annotation";
char* ATTR_SOURCE_FBX = "source_fbx";
char* ATTR_ANIM_LENGTH = "animationLength";
XmlHelper::XmlHelper() {}

XmlHelper::~XmlHelper() {}

void XmlHelper::parseXml(const std::string filePath) {
/*	
	std::string line, text;
	std::ifstream file;

	file.open(filePath);
	if (file.is_open()) {
		while (std::getline(file,line) ) {
			text.append(line);
		}
		file.close();
	}
	char * chars = new char[text.size() + 1];
	std::copy(text.begin(), text.end(), chars);
*/
	mXmlDocument.load_file(filePath.c_str());

//	commented due to pointers to text in xml_document
//	delete [] chars;
}

struct XmlFbxNode {
	XmlFbxNode::XmlFbxNode(FbxNode* _fbxNode, const std::string _fbxPath, pugi::xml_node _xmlNode)
		: fbxNode(_fbxNode), fbxPath(_fbxPath), xmlNode(_xmlNode) {};

public:
	FbxNode* fbxNode;
	const std::string fbxPath;
	pugi::xml_node xmlNode;
};

void XmlHelper::createDocument(FbxScene* scene, const std::string filePath) {
	std::stack<XmlFbxNode> stack;
	
	auto doctype = mXmlDocument.append_child(pugi::node_doctype);
	doctype.set_value("scene SYSTEM \"scene.dtd\"");

	auto nodeScene = mXmlDocument.append_child(NODE_SCENE);
	nodeScene.append_attribute(ATTR_SOURCE_FBX) = filePath.c_str();

	stack.push(XmlFbxNode(scene->GetRootNode(), "", nodeScene.append_child(NODE_STRUCTURE)));

	while (!stack.empty()) {
		auto node = stack.top();
		stack.pop();

		auto xmlNode = node.xmlNode.append_child(NODE_NODE);
		
		auto path = node.fbxPath + "/" + node.fbxNode->GetName();

		xmlNode.append_attribute(ATTR_NAME) = node.fbxNode->GetName();
		xmlNode.append_attribute(ATTR_PATH) = path.c_str();
		xmlNode.append_attribute(ATTR_ANNOTATION) = "None";

//		std::cout<< "Processing " << node.fbxNode->GetName() << "\n";
		for (int i = 0; i < node.fbxNode->GetChildCount(); i++) {
			auto child = node.fbxNode->GetChild(i);
//			std::cout << "pushing " << child->GetName() << "\n";
			stack.push(XmlFbxNode(child, path, xmlNode));
		}
	}
}

void XmlHelper::printXml(const std::string filePath) {
	if (!filePath.empty()) {
		mXmlDocument.save_file(filePath.c_str());
	} else {
		mXmlDocument.save_file(mXmlFilePath);
	}
}

std::string XmlHelper::getSourceFbx() {
	auto scene = mXmlDocument.child(NODE_SCENE);
	return scene.attribute(ATTR_SOURCE_FBX).value();
}

pugi::xml_node XmlHelper::getAnnotation() {
	return mXmlDocument.child(NODE_SCENE).child(NODE_ANNOTATION);
}

std::vector<std::string> XmlHelper::getAnnotations() {
	std::vector<std::string> annotation;
	std::stack<pugi::xml_node> stack;

	stack.push(getAnnotation().child(NODE_NODE));

	while (!stack.empty()) {
		auto node = stack.top();
		stack.pop();

		annotation.push_back(node.attribute(ATTR_PATH).value());
		for (pugi::xml_node child = node.child(NODE_NODE); child; child = child.next_sibling(NODE_NODE)) {
			stack.push(child);
		}	
	}

	return annotation;
}