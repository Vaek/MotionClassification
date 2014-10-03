
#ifndef SCENE_H
#define SCENE_H

#include <fbxsdk.h>
#include <string>
#include "Skeleton.h"
#include "Motion.h"
#include "XmlHelper.h"

class Scene {
public:
	Scene();
	~Scene();

	void exportFbxSceneStructure(const std::string filePath, FbxManager* manager);
	void loadAnnotatedScene(const std::string annotatedPath, FbxManager* manager);
	void exportSkeleton(const std::string filePath);
	void exportMotion(const std::string filePath);
	std::vector<MotionFrame> extractMotionKeyFrames();

	Skeleton* getSkeleton();
	Motion* getMotion();

private:
	Skeleton* skeleton;
	Motion* motion;

	FbxScene* loadFbxSceneFile(const std::string filePath, FbxManager* manager);
};

#endif SCENE_H
