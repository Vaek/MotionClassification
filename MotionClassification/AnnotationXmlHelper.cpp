
#include "AnnotationXmlHelper.h"
#include <stack>
#include "XmlNode.h"

const char* AnnotationXmlHelper::NODE_SCENE = "scene";
const char* AnnotationXmlHelper::NODE_STRUCTURE = "structure";
const char* AnnotationXmlHelper::NODE_ANNOTATION = "annotation";
const char* AnnotationXmlHelper::ATTR_SOURCE_FBX = "source_fbx";
const char* AnnotationXmlHelper::ATTR_ANIM_LENGTH = "animationLength";

AnnotationXmlHelper::AnnotationXmlHelper() : XmlHelper() {}

AnnotationXmlHelper::~AnnotationXmlHelper() {}

void AnnotationXmlHelper::createDoctype() {
	auto doctype = mXmlDocument.append_child(pugi::node_doctype);
	doctype.set_value("scene SYSTEM \"scene.dtd\""); 
}

pugi::xml_node AnnotationXmlHelper::getAnnotation() {
	return mXmlDocument.child(NODE_SCENE).child(NODE_ANNOTATION);
}

std::vector<std::pair<std::string, std::string>> AnnotationXmlHelper::getAnnotations() {
	std::vector<std::pair<std::string, std::string>> annotation;
	std::stack<pugi::xml_node> stack;

	stack.push(getAnnotation().child(XmlNode::NODE_NODE));

	while (!stack.empty()) {
		auto node = stack.top();
		stack.pop();

		annotation.push_back(std::pair<std::string, std::string>(node.attribute(XmlNode::ATTR_PATH).value(), node.attribute(XmlNode::ATTR_ANNOTATION).value()));
		for (pugi::xml_node child = node.child(XmlNode::NODE_NODE); child; child = child.next_sibling(XmlNode::NODE_NODE)) {
			stack.push(child);
		}
	}

	return annotation;
}

std::string AnnotationXmlHelper::getSourceFilePath() {
	auto scene = mXmlDocument.child(NODE_SCENE);
	return scene.attribute(ATTR_SOURCE_FBX).value();
}