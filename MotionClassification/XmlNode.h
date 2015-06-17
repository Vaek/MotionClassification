
#ifndef XMLNODE_H
#define XMLNODE_H

#include <vector>
#include "pugi/pugixml.hpp"

class XmlNode {
public:
	XmlNode(pugi::xml_node _parent);
	~XmlNode();

	void createXmlNode();

	static const char* NODE_NODE;
	static const char* ATTR_NAME;
	static const char* ATTR_PATH;
	static const char* ATTR_ANNOTATION;

protected:
	pugi::xml_node createChild(const char* name, const char* path, const char* annotation);
	pugi::xml_node parent;
};

#endif // !XMLNODE_H

