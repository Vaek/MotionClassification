
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
std::vector<Scene*> loadedScenes;

// Clean up function called on end of program
void clearLoadedScenes() {
	for each (Scene* scene in loadedScenes) delete scene;
	loadedScenes.clear();
}

static void cleanUp() {

	// Destroy the SDK manager and all the other objects it was handling.
	manager->Destroy();
	clearLoadedScenes();
}

static void initFbxManager() {

	// Initialize the SDK manager. This object handles all our memory management.
	manager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings *ioSettings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ioSettings);
}

void exportStructure(const std::string motionFilePath) {
	Scene scene;
	scene.exportSceneStructure(motionFilePath, manager);
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
			  << "clear loaded scenes\t\t\t4" << std::endl
			  << "exit\t\t\t\t\t-1" << std::endl << std::endl;
}

void loop(){
	Scene* lastLoadedScene = nullptr;
	do {
		printHelp();

		std::string input;
		std::cin >> input;

		int toDo = -1;
		if (!input.empty()) {
			toDo = std::atoi(input.c_str());
		}
		
		switch (toDo) {
		case -1:
			return;

		case 0: {
			std::string fbxPath;
			std::cin >> fbxPath;
			exportStructure(fbxPath);
			break;
		}

		case 1: {
			std::string annotationPath;
			std::cin >> annotationPath;
			lastLoadedScene = loadScene(annotationPath);
			loadedScenes.push_back(lastLoadedScene);
			break;
		}

		case 2: {
			std::string motionClass;
			std::cin >> motionClass;
			if (loadedScenes.size() > 1) {
				std::vector<MotionObject> keyFrames;
				for each (Scene* scene in loadedScenes) keyFrames.push_back(scene->extractMotionKeyFrames());
				container.combineAndUpdateLearnMotion(motionClass, keyFrames);
			} else if (false && lastLoadedScene != nullptr) {
				container.updateLearnMotion(motionClass, lastLoadedScene->extractMotionKeyFrames());
			}
			break;
		}

		case 3:
			std::cout << "Best motion class is " << container.recognizeMotionClass(lastLoadedScene->extractMotionKeyFrames()) << std::endl;
			break;

		case 4:
			clearLoadedScenes();
			break;

		default:
			std::cout << "Unknown first parameter.";
			break;
		}

		std::cout << "--------------------------------------- " << std::endl << std::endl;
	} while (true);
}

int ARG_INDEX_LOAD_LEARNED_DATA = 1;
int ARG_INDEX_TODO = 2;

// todo == 0
int ARG_INDEX_MOTION_FILE = 3;

// todo = 1
int ARG_INDEX_MOTION_CLASS_NAME = 3;
int ARG_INDEX_ANNOTATED_FILE_FIRST = 4;

// todo == otherwise
// run custome loop

// Main
int main(int argc, char** argv) {

	std::cout << "Run with " << argc << " parameters: ";
	for (size_t i = 1; i < argc; i++) std::cout << argv[i] << " ";
	std::cout << std::endl;

	// init manager
	initFbxManager();

	// load learned data
	bool load = false;
	if (argc > 1 && atoi(argv[ARG_INDEX_LOAD_LEARNED_DATA]) == 1) load = true;
	if (load) container.loadLearnedData();

	int toDo = INT_MAX;
	if (argc > 2) toDo = atoi(argv[ARG_INDEX_TODO]);

	switch (toDo) {
	case 0:
		exportStructure(argv[ARG_INDEX_MOTION_FILE]);
		break;

	case 1: {
		// learn annotated files
		for (int i = ARG_INDEX_ANNOTATED_FILE_FIRST; i < argc; i++) {
			loadedScenes.push_back(loadScene(argv[i]));
		}
		std::vector<MotionObject> keyFrames;
		if (loadedScenes.size() > 1) {
			for each (Scene* scene in loadedScenes) {
				keyFrames.push_back(scene->extractMotionKeyFrames());
			}
		}
		container.combineAndUpdateLearnMotion(argv[ARG_INDEX_MOTION_CLASS_NAME], keyFrames);
		break;
			}

	default:
		loop();
		break;
	}

	if (load) {
		std::cout << "Save learned data? [y/n]" << std::endl;
		std::string save;
		std::cin >> save;
		if (save == "y" || save == "Y") container.saveLearnedData();
	}

	cleanUp();	
	return 0;
}
