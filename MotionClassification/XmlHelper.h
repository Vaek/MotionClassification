#ifndef XML_HELPER_H
#define XML_HELPER_H

#include <string>
#include "pugi\pugixml.hpp"

class XmlHelper {
public:
	XmlHelper();
	~XmlHelper();
	
	void parseXml(const std::string filePath);
	void printXml(const std::string filePath);

protected:
	const std::string mXmlFilePath;
	pugi::xml_document mXmlDocument;
};

#endif // !XML_HELPER_H
