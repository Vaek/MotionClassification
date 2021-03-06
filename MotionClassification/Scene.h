
#ifndef SCENE_H
#define SCENE_H

#include <fbxsdk.h>
#include <string>
#include "Skeleton.h"
#include "Motion.h"
#include "MotionObject.h"

class Scene {
public:
	Scene();
	~Scene();

	std::string exportSceneStructure(const std::string filePath, FbxManager* manager);
	void loadAnnotatedScene(const std::string annotatedPath, FbxManager* manager);
	void exportSkeleton(const std::string filePath);
	void exportMotion(const std::string filePath);
	MotionObject extractMotionKeyFrames();

	Skeleton* getSkeleton();
	Motion* getMotion();

private:
	Skeleton* skeleton;
	Motion* motion;

	FbxScene* loadFbxSceneFile(const std::string filePath, FbxManager* manager);
};

#endif SCENE_H
