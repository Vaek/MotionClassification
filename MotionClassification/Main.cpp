
#include <iostream>
#include <fstream>

// Math library glm
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <fbxsdk.h>
#include "FbxSceneLoader.h"
#include "FbxSceneExporter.h"
#include "DataConvertor.h"

// memory manager for FBX files
FbxManager* manager;

// data
std::string dirPath = "./";
//std::string fbxPath = dirPath+"skeleton.fbx";
std::string fbxPath = dirPath+"pepe_mocap.fbx";

// Clean up function called on end of program
static void cleanUp() {

	// Destroy the SDK manager and all the other objects it was handling.
	manager->Destroy();
}

static void initFbxManager() {

	// Initialize the SDK manager. This object handles all our memory management.
	manager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings *ioSettings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ioSettings);
}

void exportFbxStructure(const std::string filePath) {
	FbxScene* scene = getScene(filePath, manager);

	if (!scene) {
		std::cout << "File not found.\n";
		return;
	}

	std::vector<node_info> nodes = getAllFbxNodesWithPaths(scene->GetRootNode());
	std::ofstream file;

	file.open("fbx_structure.txt");
	if (file.is_open()) {
		for (const auto node: nodes) {
			file << node.second << std::endl;
		}
		file.close();
	}
}

std::vector<std::string> getPathsAnnotatedNodes(const std::string filePath) {
	std::vector<std::string> nodePaths;
	std::string line;
	std::ifstream file;

	file.open(filePath);
	if (file.is_open()) {
		while (std::getline(file,line) ) {
			nodePaths.push_back(line);
		}
		file.close();
	}

	return nodePaths;
}

void loadScene(const std::string filePath, const std::string annotationPath) {
	FbxScene* scene = getScene(filePath, manager);
	
	auto pathsAnnotatedNodes = getPathsAnnotatedNodes(annotationPath);
	auto annotatedNodes = findAnnotatedNodes(scene->GetRootNode(), pathsAnnotatedNodes);
	Skeleton* skeleton = fbxToSkeleton(scene, annotatedNodes);
	Motion* motion = fbxToMotion(scene, annotatedNodes);
	
	if (skeleton) {
		std::ofstream skeletonFile;
		skeletonFile.open("skeleton.txt");
	
		if (skeletonFile.is_open()) {
			skeletonFile << *skeleton->getRoot();
			skeletonFile.close();
		}
	}
	if (motion) {
		std::ofstream motionFile;
		motionFile.open("motion.txt");
	
		if (motionFile.is_open()) {
			motionFile << *motion;
			motionFile.close();
		}
	}
}

// Main
int main(int argc, char** argv) {

	// init manager
	initFbxManager();
	
	switch (argc-1) {
	case 1:
		fbxPath = argv[1];
		exportFbxStructure(fbxPath);
		break;

	case 2: {
		fbxPath = argv[1];
		std::string annotationPath = argv[2];
		loadScene(fbxPath, annotationPath);
		break;
			}
	default:
		std::cout << "Unknown input parameters.\n";
		break;
	}
	
	cleanUp();
	return 0;

	// load data
	FbxScene* scene = getScene(fbxPath, manager);
/*
	for (auto&child_pair: nodes) {
		auto child = child_pair.first;
//		std::cout << child_pair.second << " is a " << child->GetTypeName();// << "\n";
//
		int attr_count = child->GetNodeAttributeCount();
		for (int a=0;a<attr_count;++a) {
//			std::cout << ", " << GetAttributeTypeName(child->GetNodeAttributeByIndex(a)->GetAttributeType());
		}
//		std::cout << "\n";
	}

	return 0;
*/
	cleanUp();	
	
	return 0;

/*
export cest ke všem nodes
definovat ze vstupu ty co jsou dùležitý pro nás 
externì seznam co musím mít v datech
*/
}
