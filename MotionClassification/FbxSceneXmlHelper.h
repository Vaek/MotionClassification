
#ifndef FBX_SCENE_XML_HELPER_H
#define FBX_SCENE_XML_HELPER_H

#include <fbxsdk.h>
#include <vector>
#include "AnnotationXmlHelper.h"

class FbxSceneXmlHelper : public AnnotationXmlHelper {
public:
	FbxSceneXmlHelper();
	~FbxSceneXmlHelper();

	void createDocument(FbxScene* data, const std::string filePath);

private:

};

#endif //!FBX_SCENE_XML_HELPER_H