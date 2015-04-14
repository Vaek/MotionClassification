
#ifndef ANNOTATION_XML_HELPER_H
#define ANNOTATION_XML_HELPER_H

#include <vector>
#include "XmlHelper.h"

class AnnotationXmlHelper : public XmlHelper {
public:
	AnnotationXmlHelper();
	~AnnotationXmlHelper();

	std::string getSourceFilePath();
	std::vector<std::string> getAnnotations();

	static const char* NODE_SCENE;
	static const char* NODE_STRUCTURE;
	static const char* NODE_ANNOTATION;
	static const char* ATTR_SOURCE_FBX;
	static const char* ATTR_ANIM_LENGTH;

private:
	pugi::xml_node getAnnotation();

protected:
	void createDoctype();
};

#endif //!ANNOTATION_XML_HELPER_H