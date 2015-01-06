
#ifndef FBX_SCENE_XML_HELPER_H
#define FBX_SCENE_XML_HELPER_H

#include <fbxsdk.h>
#include <vector>
#include "XmlHelper.h"

class FbxSceneXmlHelper : public XmlHelper {
public:
	FbxSceneXmlHelper();
	~FbxSceneXmlHelper();

	void createDocument(FbxScene* data, const std::string filePath);
	std::string getSourceFbx();
	std::vector<std::string> getAnnotations();

private:
	pugi::xml_node getAnnotation();

};

#endif //!FBX_SCENE_XML_HELPER_H