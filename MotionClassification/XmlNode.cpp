
#include "XmlNode.h"

const char* XmlNode::NODE_NODE = "node";
const char* XmlNode::ATTR_NAME = "name";
const char* XmlNode::ATTR_PATH = "path";
const char* XmlNode::ATTR_ANNOTATION = "annotation";

XmlNode::XmlNode(pugi::xml_node _parent) : parent(_parent) {
//	createXmlNode();
}

XmlNode::~XmlNode() {}

void XmlNode::createXmlNode() {}

pugi::xml_node XmlNode::createChild(const char* name, const char* path, const char* annotation) {
	auto xmlChild = this->parent.append_child(XmlNode::NODE_NODE);

	xmlChild.append_attribute(XmlNode::ATTR_NAME) = name;
	xmlChild.append_attribute(XmlNode::ATTR_PATH) = path;
	xmlChild.append_attribute(XmlNode::ATTR_ANNOTATION) = annotation;

	return xmlChild;
}