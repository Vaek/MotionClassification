
#include <fstream>
#include "Scene.h"
#include "DataConvertor.h"
#include "FbxSceneLoader.h"
#include "KeyFrameExtractor.h"
#include "FbxSceneXmlHelper.h"
//#include "BvhSceneXmlHelper.h"
//#include "bvh\bvh.h"

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

MotionObject Scene::extractMotionKeyFrames() {
	auto mo = extractKeyFrames(getMotion());
	if (this->skeleton) {
		for each (auto offsetPair in this->skeleton->getNodeOfssets()) {
			mo.setNodeOffset(offsetPair);
		}			
	}
	return mo;
}

FbxScene* Scene::loadFbxSceneFile(const std::string filePath, FbxManager* manager) {
	auto fbxScene = getScene(filePath, manager);

	if (!fbxScene) {
		std::cout << "File not found.\n";
		return nullptr;
	}
	return fbxScene;
}

void createDtd(std::string filePath) {
	std::ofstream dtdFile;
	dtdFile.open(filePath);

	if (dtdFile.is_open()) {
		dtdFile << "<!ELEMENT scene (structure?, annotation?)>" << std::endl <<
			"<!ELEMENT structure (node)>" << std::endl <<
			"<!ELEMENT node (node*)>" << std::endl <<
			"<!ELEMENT annotation (node)>" << std::endl <<
			"" << std::endl <<
			"<!ATTLIST scene source_fbx CDATA #REQUIRED>" << std::endl <<
			"<!ATTLIST node name CDATA #REQUIRED>" << std::endl <<
			"<!ATTLIST node path CDATA #REQUIRED>" << std::endl <<
			"<!ATTLIST node animationLength CDATA #REQUIRED>" << std::endl <<
			"<!ATTLIST node annotation(None | Hips | Spin | Spin1 | Spin2 | Neck | Head | RightShoulder | RightArm | RightArmRoll | RightForeArm | RightForeArmRoll |" << std::endl <<
			"RightHand | RightHandThumb1 | RightHandThumb2 | RightHandThumb3 | RightHandThumb4 | RightHandIndex1 | RightHandIndex2 |" << std::endl <<
			"RightHandIndex3 | RightHandIndex4 | RightHandMiddle1 | RightHandMiddle2 | RightHandMiddle3 | RightHandMiddle4 | RightHandRing1 |" << std::endl <<
			"RightHandRing2 | RightHandRing3 | RightHandRing4 | RightHandPinky1 | RightHandPinky2 | RightHandPinky3 | RightHandPinky4 |" << std::endl <<
			"LeftShoulder | LeftArm | LeftArmRoll | LeftForeArm | LeftForeArmRoll | LeftHand | LeftHandThumb1 | LeftHandThumb2 | LeftHandThumb3 |" << std::endl <<
			"LeftHandThumb4 | LeftHandIndex1 | LeftHandIndex2 | LeftHandIndex3 | LeftHandIndex4 | LeftHandMiddle1 | LeftHandMiddle2 |" << std::endl <<
			"LeftHandMiddle3 | LeftHandMiddle4 | LeftHandRing1 | LeftHandRing2 | LeftHandRing3 | LeftHandRing4 | LeftHandPinky1 |" << std::endl <<
			"LeftHandPinky2 | LeftHandPinky3 | LeftHandPinky4 | RightHip | RightUpLeg | RightUpLegRoll | RightLeg | RightLegRoll | RightFoot |" << std::endl <<
			"RightToeBase | LeftHip | LeftUpLeg | LeftUpLegRoll | LeftLeg | LeftLegRoll | LeftFoot | LeftToeBase) \"None\">" << std::endl;
		dtdFile.close();
	}
}

std::string Scene::exportSceneStructure(const std::string filePath, FbxManager* manager) {
	std::string exportFilePath = filePath.substr(0, filePath.find_last_of(".") + 1).append("xml");

//	if (true || filePath.find(".fbx") != std::string::npos) {
		auto scene = loadFbxSceneFile(filePath, manager);
		if (!scene) {
			std::cout << "Can not load file to export." << std::endl;
			return "";
		}

		FbxSceneXmlHelper helper;
		helper.createDocument(scene, filePath);
		helper.printXml(exportFilePath);
		std::cout << "Structure is stored in: " << exportFilePath << std::endl;
		std::string dtdFilePath;
		if (filePath.find_last_of("/") != std::string::npos) {
			dtdFilePath = filePath.substr(0, filePath.find_last_of("/") + 1).append("scene.dtd");
		} else if (filePath.find_last_of("\\") != std::string::npos) {
			dtdFilePath = filePath.substr(0, filePath.find_last_of("\\") + 1).append("scene.dtd");
		} else {
			dtdFilePath = "./scene.dtd";
		}
		createDtd(dtdFilePath);
/*
	} else if (false && filePath.find(".bvh") != std::string::npos) {
		Bvh scene;
		if (!scene.load(filePath)) {
			std::cout << "Can not load file to export." << std::endl;
			return;
		}

		BvhSceneXmlHelper helper;
		helper.createDocument(scene, filePath);
		helper.printXml(exportFilePath);
		std::cout << "Structure is stored in: " << exportFilePath << std::endl;
	}
*/
		return exportFilePath;
}

void Scene::loadAnnotatedScene(const std::string annotatedPath, FbxManager* manager) {
	FbxSceneXmlHelper helper;
	helper.parseXml(annotatedPath);
	auto fbxPath = helper.getSourceFilePath();
	if (!fbxPath.empty()) {
		auto scene = loadFbxSceneFile(fbxPath, manager);
		if (!scene) {
			std::cout << "Can not load file to export." << std::endl;
			return;
		}

		auto pathsAnnotatedNodes = helper.getAnnotations();
		if (pathsAnnotatedNodes.size() == 0) std::cout << "!!! WARNING: No annotated nodes to load." << std::endl;
		auto annotatedNodes = findAnnotatedNodes(scene->GetRootNode(), pathsAnnotatedNodes);

		this->skeleton = fbxToSkeleton(scene, annotatedNodes, pathsAnnotatedNodes);
		this->motion = fbxToMotion(scene, annotatedNodes, pathsAnnotatedNodes);

//		exportSkeleton("skeleton.txt");
//		exportMotion("motion.txt");
	} else {
		std::cout << "Fbx not found." << std::endl;
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