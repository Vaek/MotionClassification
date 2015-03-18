
#include <iostream>
#include <fstream>

// Math library glm
//#include <glm/glm.hpp> 
//#include <glm/gtc/type_ptr.hpp>

#include <fbxsdk.h>
#include "Scene.h"
#include "LearnDataContainer.h"

// memory manager for FBX files
FbxManager* manager;
LearnDataContainer container;

// data
//std::string dirPath = "./";
//std::string fbxPath = dirPath+"skeleton.fbx";
//std::string fbxPath = dirPath+"pepe_mocap.fbx";

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

void exportFbxStructure(const std::string fbxPath) {
	Scene scene;
	scene.exportFbxSceneStructure(fbxPath, manager);
}


Scene* loadScene(const std::string annotationPath) {
	Scene* scene = new Scene();
	scene->loadAnnotatedScene(annotationPath, manager);
	return scene;
}

void printHelp() {	
	std::cout << "export fbx structuret\t\t\t0 [fbxFile]" << std::endl
			  << "load fbx with annotated file\t\t1 [annotationFile]" << std::endl
			  << "learn motion data\t\t\t2 [motionClass]" << std::endl
			  << "recognize motion data\t\t\t3" << std::endl
			  << "exit\t\t\t\t\t-1" << std::endl << std::endl;
}

void loop(){
	Scene* lastLoadedScene = nullptr;
	do {
		printHelp();

		int toDo;
		std::cin >> toDo;
		
		switch (toDo) {
		case -1:
			return;

		case 0: {
			std::string fbxPath;
			std::cin >> fbxPath;
			exportFbxStructure(fbxPath);
			break;
				}

		case 1: {
			std::string annotationPath;
			std::cin >> annotationPath;
			if (lastLoadedScene != nullptr) delete lastLoadedScene;
			lastLoadedScene = loadScene(annotationPath);
			break;
				}

		case 2: {
			std::string motionClass;
			std::cin >> motionClass;
			if (lastLoadedScene != nullptr) {
				container.updateLearnMotion(motionClass, lastLoadedScene->extractMotionKeyFrames());
			}
			break;
				}

		case 3:
			std::cout << "Best motion class is " << container.recognizeMotionClass(lastLoadedScene->extractMotionKeyFrames()) << std::endl;
			break;

		default:
			std::cout << "Unknown first parameter.";
			break;
		}

		std::cout << "--------------------------------------- " << std::endl << std::endl;
	} while (true);
}

// Main
int main(int argc, char** argv) {

	// init manager
	initFbxManager();
	
	int toDo = INT_MAX;
	if (argc-1 > 0) toDo = atoi(argv[1]);

	container.loadLearnedData();

	switch (toDo) {
	case 0:
		exportFbxStructure(argv[2]);
		break;

	case 1: {
		loadScene(argv[3]);
		break;
			}

	default:
		loop();
		break;
	}

	std::cout << "Save learned data? [y/n]" << std::endl;
	std::string save;
	std::cin >> save;
	if (save == "y" || save == "Y") container.saveLearnedData();

	cleanUp();	
	return 0;

/*
externì seznam co musím mít v datech
*/
}
