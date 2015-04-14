
#ifndef BVH_SCENE_XML_HELPER_H
#define BVH_SCENE_XML_HELPER_H

#include "bvh\bvh.h"
#include <vector>
#include "AnnotationXmlHelper.h"

class BvhSceneXmlHelper : public AnnotationXmlHelper {
public:
	BvhSceneXmlHelper();
	~BvhSceneXmlHelper();

	void createDocument(const Bvh& data, const std::string filePath);

private:

};

#endif //!BVH_SCENE_XML_HELPER_H