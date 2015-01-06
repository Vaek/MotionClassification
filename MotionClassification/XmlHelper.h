#ifndef XML_HELPER_H
#define XML_HELPER_H

#include <string>
#include <vector>
#include "pugi\pugixml.hpp"
#include <fbxsdk.h>

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

protected:
	const std::string mXmlFilePath;
	pugi::xml_document mXmlDocument;

	pugi::xml_node getAnnotation();

};

#endif // !XML_HELPER_H
