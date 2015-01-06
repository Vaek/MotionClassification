
#include <fstream>
#include "Scene.h"
#include "DataConvertor.h"
#include "FbxSceneLoader.h"
#include "KeyFrameExtractor.h"
#include "FbxSceneXmlHelper.h"

Scene::Scene() {
	this->skeleton = nullptr;
	this->motion = nullptr;
}

Scene::~Scene() {
	delete this->skeleton;
	delete this->motion;
}

Skeleton* Scene::getSkeleton() {
	return this->skeleton;
}

Motion* Scene::getMotion() {
	return this->motion;
}

std::vector<MotionFrame> Scene::extractMotionKeyFrames() {
	return extractKeyFrames(getMotion());
}

FbxScene* Scene::loadFbxSceneFile(const std::string filePath, FbxManager* manager) {
	auto fbxScene = getScene(filePath, manager);

	if (!fbxScene) {
		std::cout << "File not found.\n";
		return nullptr;
	}
	return fbxScene;
}

void Scene::exportFbxSceneStructure(const std::string filePath, FbxManager* manager) {
	auto scene = loadFbxSceneFile(filePath, manager);
	if (!scene) {
		std::cout << "Can not load file to export." << std::endl;
		return;
	}

	std::string exportFilePath = filePath.substr(0, filePath.find_last_of(".")+1).append("xml");

	FbxSceneXmlHelper helper;
	helper.createDocument(scene, filePath);
	helper.printXml(exportFilePath);
	std::cout << "Structure is stored in: " << exportFilePath << std::endl;
}

void Scene::loadAnnotatedScene(const std::string annotatedPath, FbxManager* manager) {
	FbxSceneXmlHelper helper;
	helper.parseXml(annotatedPath);
	auto fbxPath = helper.getSourceFbx();
	if (!fbxPath.empty()) {
		auto scene = loadFbxSceneFile(fbxPath, manager);
		if (!scene) {
			std::cout << "Can not load file to export." << std::endl;
			return;
		}

		auto pathsAnnotatedNodes = helper.getAnnotations();
		auto annotatedNodes = findAnnotatedNodes(scene->GetRootNode(), pathsAnnotatedNodes);

		this->skeleton = fbxToSkeleton(scene, annotatedNodes);
		this->motion = fbxToMotion(scene, annotatedNodes);

//		exportSkeleton("skeleton.txt");
//		exportMotion("motion.txt");
	}
}

void Scene::exportSkeleton(const std::string filePath) {
	if (skeleton) {
		std::ofstream skeletonFile;
		skeletonFile.open(filePath);
	
		if (skeletonFile.is_open()) {
			if (getSkeleton()) skeletonFile << *(getSkeleton()->getRoot());			
			skeletonFile.close();
		}
	}
}

void Scene::exportMotion(const std::string filePath) {
	if (motion) {
		std::ofstream motionFile;
		motionFile.open(filePath);
	
		if (motionFile.is_open()) {
			if (getMotion()) motionFile << *getMotion();
			motionFile.close();
		}
	}
}