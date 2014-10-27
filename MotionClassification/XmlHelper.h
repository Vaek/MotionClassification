#ifndef XML_ADAPTER_H
#define XML_ADAPTER_H

#include <string>
#include <fbxsdk.h>
#include <vector>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "pugi/pugixml.hpp"

class XmlHelper {
public:
	XmlHelper();
	~XmlHelper();
	
	void parseXml(const std::string filePath);
	void createDocument(FbxScene* scene, const std::string filePath);
	void printXml(const std::string filePath = nullptr);
	std::string getSourceFbx();
	std::vector<std::string> getAnnotations();

	pugi::xml_document getDocument();

private:
	const char* mXmlFilePath;
	pugi::xml_document mXmlDocument;

	pugi::xml_node getAnnotation();
};

#endif // !XML_ADAPTER_H
