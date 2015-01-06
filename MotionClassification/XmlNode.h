
#ifndef XMLNODE_H
#define XMLNODE_H

#include <vector>
#include "pugi\pugixml.hpp"

class XmlNode {
public:
	XmlNode(pugi::xml_node _parent);
	~XmlNode();

	void createXmlNode();

protected:
	pugi::xml_node parent;
};

#endif // !XMLNODE_H

