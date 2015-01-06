
#include "XmlHelper.h"
#include <fstream>
#include <vector>
#include <iostream>

XmlHelper::XmlHelper() {}

XmlHelper::~XmlHelper() {}

void XmlHelper::parseXml(const std::string filePath) {
//	this->mXmlFilePath = filePath;
	mXmlDocument.load_file(filePath.c_str());

	//    commented due to pointers to text in xml_document
	//    delete [] chars;
}

void XmlHelper::printXml(const std::string filePath) {
	if (!filePath.empty()) {
		mXmlDocument.save_file(filePath.c_str());
	} else {
		mXmlDocument.save_file(this->mXmlFilePath.c_str());
	}
}
