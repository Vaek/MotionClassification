
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
		for (auto offsetPair: this->skeleton->getNodeOfssets()) {
			mo.setNodeOffset(offsetPair);
		}			
	}
	return mo;
}

FbxScene* Scene::loadFbxSceneFile(const std::string filePath, FbxManager* manager) {
	auto fbxScene = getScene(filePath, manager);

	if (!fbxScene) {
		std::clog << "File not found.\n";
		return nullptr;
	}
	return fbxScene;
}

void createDtd(std::string filePath) {
	std::ofstream dtdFile;
	dtdFile.open(filePath);

	if (dtdFile.is_open()) {
		dtdFile << "<!ELEMENT scene (structure?, annotation?)>" << "\n" <<
			"<!ELEMENT structure (node)>" << "\n" <<
			"<!ELEMENT node (node*)>" << "\n" <<
			"<!ELEMENT annotation (node)>" << "\n" <<
			"" << "\n" <<
			"<!ATTLIST scene source_fbx CDATA #REQUIRED>" << "\n" <<
			"<!ATTLIST node name CDATA #REQUIRED>" << "\n" <<
			"<!ATTLIST node path CDATA #REQUIRED>" << "\n" <<
			"<!ATTLIST node animationLength CDATA #REQUIRED>" << "\n" <<
			"<!ATTLIST node annotation(None | Hips | Spin | Spin1 | Spin2 | Neck | Head | RightShoulder | RightArm | RightArmRoll | RightForeArm | RightForeArmRoll |" << "\n" <<
			"RightHand | RightHandThumb1 | RightHandThumb2 | RightHandThumb3 | RightHandThumb4 | RightHandIndex1 | RightHandIndex2 |" << "\n" <<
			"RightHandIndex3 | RightHandIndex4 | RightHandMiddle1 | RightHandMiddle2 | RightHandMiddle3 | RightHandMiddle4 | RightHandRing1 |" << "\n" <<
			"RightHandRing2 | RightHandRing3 | RightHandRing4 | RightHandPinky1 | RightHandPinky2 | RightHandPinky3 | RightHandPinky4 |" << "\n" <<
			"LeftShoulder | LeftArm | LeftArmRoll | LeftForeArm | LeftForeArmRoll | LeftHand | LeftHandThumb1 | LeftHandThumb2 | LeftHandThumb3 |" << "\n" <<
			"LeftHandThumb4 | LeftHandIndex1 | LeftHandIndex2 | LeftHandIndex3 | LeftHandIndex4 | LeftHandMiddle1 | LeftHandMiddle2 |" << "\n" <<
			"LeftHandMiddle3 | LeftHandMiddle4 | LeftHandRing1 | LeftHandRing2 | LeftHandRing3 | LeftHandRing4 | LeftHandPinky1 |" << "\n" <<
			"LeftHandPinky2 | LeftHandPinky3 | LeftHandPinky4 | RightHip | RightUpLeg | RightUpLegRoll | RightLeg | RightLegRoll | RightFoot |" << "\n" <<
			"RightToeBase | LeftHip | LeftUpLeg | LeftUpLegRoll | LeftLeg | LeftLegRoll | LeftFoot | LeftToeBase) \"None\">" << "\n";
		dtdFile.close();
	}
}

std::string Scene::exportSceneStructure(const std::string filePath, FbxManager* manager) {
	std::string exportFilePath = filePath.substr(0, filePath.find_last_of(".") + 1).append("xml");

//	if (true || filePath.find(".fbx") != std::string::npos) {
		auto scene = loadFbxSceneFile(filePath, manager);
		if (!scene) {
			std::clog << "Can not load file to export." << "\n";
			return "";
		}

		FbxSceneXmlHelper helper;
		helper.createDocument(scene, filePath);
		helper.printXml(exportFilePath);
		std::clog << "Structure is stored in: " << exportFilePath << "\n";
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
			std::clog << "Can not load file to export." << "
";
			return;
		}

		BvhSceneXmlHelper helper;
		helper.createDocument(scene, filePath);
		helper.printXml(exportFilePath);
		std::clog << "Structure is stored in: " << exportFilePath << "
";
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
			std::clog << "Can not load file to export." << "\n";
			return;
		}

		auto pathsAnnotatedNodes = helper.getAnnotations();
		if (pathsAnnotatedNodes.size() == 0) std::clog << "!!! WARNING: No annotated nodes to load." << "\n";
		auto annotatedNodes = findAnnotatedNodes(scene->GetRootNode(), pathsAnnotatedNodes);

		this->skeleton = fbxToSkeleton(scene, annotatedNodes, pathsAnnotatedNodes);
		this->motion = fbxToMotion(scene, annotatedNodes, pathsAnnotatedNodes);

//		exportSkeleton("skeleton.txt");
//		exportMotion("motion.txt");
	} else {
		std::clog << "Fbx not found." << "\n";
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
