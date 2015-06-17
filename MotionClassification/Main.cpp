
#include <iostream>
#include <fstream>
#include <iomanip>
#include <climits>
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
	for (Scene* scene: loadedScenes) delete scene;
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

std::string exportStructure(const std::string motionFilePath) {
	Scene scene;
	return scene.exportSceneStructure(motionFilePath, manager);
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

void recognizeMotion(Scene* scene) {
	if (scene) {
		auto recognizes = container.recognizeMotionClass(scene->extractMotionKeyFrames());
		std::cout << "Recognized motions:" << std::endl;
		if (recognizes.empty()) {
			std::cout << "\tNo motions recognized!" << std::endl;
		} else {
			for (auto recognize: recognizes) {
				std::cout << "\t" << std::setw(17) << recognize.second << " from: " << recognize.first.first << " to: " << recognize.first.second << std::endl;
			}
		}
	} else {
		std::cout << "No motion to recognize." << std::endl;
	}
}

void recognizeMotion(std::string filePath) {
	if (filePath.find(".xml") == std::string::npos) {
		filePath = exportStructure(filePath);
		if (filePath.empty()) return;
		std::cout << "File you passed is not annotated. In file " << filePath << " is created structure. Set Annotation and then click to continue." << std::endl;
		std::cin.get();
	}
	recognizeMotion(loadScene(filePath));
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
				for (Scene* scene: loadedScenes) keyFrames.push_back(scene->extractMotionKeyFrames());
				container.combineAndUpdateLearnMotion(motionClass, keyFrames);
			} else if (false && lastLoadedScene != nullptr) {
				container.updateLearnMotion(motionClass, lastLoadedScene->extractMotionKeyFrames());
			}
			break;
		}

		case 3: {
			recognizeMotion(lastLoadedScene);
			break;
		}

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
int ARG_INDEX_SAVE_LEARNED_DATA = 2;
int ARG_INDEX_TODO = 3;

// todo == 0 -> export file structure
int ARG_INDEX_MOTION_FILE = 4;

// todo == 1 -> learn motion from annotated files
int ARG_INDEX_MOTION_CLASS_NAME = 4;
int ARG_INDEX_ANNOTATED_FILE_FIRST = 5;

// todo == 2 -> recognize motion
int ARG_INDEX_MOTION_FILE2 = 4;

// todo == 3 -> run custome loop
// run custome loop

/*
1 1 1 run "./data/run/51 - run.xml" "./data/run/52 - run.xml" "./data/run/53 - run.xml" "./data/run/54 - run.xml" "./data/run/55 - run.xml"
1 1 1 step_left "./data/step_left/15 - walk_left.xml" "./data/step_left/16 - walk_left.xml" "./data/step_left/21 - walk_left.xml" "./data/step_left/22 - walk_left.xml" "./data/step_left/31 - walk_left.xml"
1 1 1 jumpf "./data/jumpf/16_05.xml" "./data/jumpf/16_06.xml" "./data/jumpf/16_07.xml" "./data/jumpf/16_09.xml" "./data/jumpf/16_10.xml"
1 0 2 "./recognize/16_28_walk_90leftturn.bvh"
1 0 2 "./recognize/16_28_walk_90leftturn.xml"
*/

class Test
{
public:
	Test(double _key) : key(_key) {};
	~Test() {};

	bool operator<(const Test& rhs) const {
		return this->key < rhs.key;
	}

	void changeKey() {
		key = key*key;
	}
private:
	double key;
};

// Main
int main(int argc, char** argv) {

	/*
	std::priority_queue<Test> tq;
	tq.push(Test(0.0));
	tq.push(Test(0.1));
	tq.push(Test(0.2));
	tq.push(Test(0.3));
	tq.push(Test(0.4));

	for (auto i = 0; i < tq.size(); i++) {
		auto tmp = tq.top();
		tq.pop();
		tmp.changeKey();
		tq.push(tmp);
	}
	*/

	std::cout << "Run with " << argc << " parameters: ";
	for (size_t i = 1; i < argc; i++) std::cout << argv[i] << " ";
	std::cout << std::endl;

	// init manager
	initFbxManager();

	// load learned data
	if (argc > ARG_INDEX_LOAD_LEARNED_DATA && atoi(argv[ARG_INDEX_LOAD_LEARNED_DATA]) == 1)  {
		container.loadLearnedData();
	}

	int toDo = INT_MAX;
	if (argc > ARG_INDEX_TODO) toDo = atoi(argv[ARG_INDEX_TODO]);

	switch (toDo) {
	case 0:
		exportStructure(argv[ARG_INDEX_MOTION_FILE]);
		break;

	case 1: {
		// learn annotated files
		const clock_t begin_time = clock();
		int scenesLengt = 0;
		for (int i = ARG_INDEX_ANNOTATED_FILE_FIRST; i < argc; i++) {
			loadedScenes.push_back(loadScene(argv[i]));
		}
		std::cout << "Extracting keyframes" << std::endl;
		std::vector<MotionObject> keyFrames;
		if (loadedScenes.size() > 1) {
			for (Scene* scene: loadedScenes) {
				scenesLengt += scene->getMotion()->getMotionLength();
				keyFrames.push_back(scene->extractMotionKeyFrames());
			}
		}
		std::cout << "Combining Motions to one" << std::endl;
		container.combineAndUpdateLearnMotion(argv[ARG_INDEX_MOTION_CLASS_NAME], keyFrames);

		std::cout << "Learn in " << float(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;
		std::cout << "Scenes " << loadedScenes.size() << std::endl;
		std::cout << "Scenes lenght " << scenesLengt / loadedScenes.size() << std::endl;
		break;
	}

	case 2: {
		recognizeMotion(argv[ARG_INDEX_MOTION_FILE2]);
		break;
	}

	case 3:
		loop();
		break;

	default:
		std::cout << "Unknown DO." << std::endl;
		break;
	}

	if (argc > ARG_INDEX_SAVE_LEARNED_DATA && atoi(argv[ARG_INDEX_SAVE_LEARNED_DATA]) == 1) {
		std::cout << "Saving learned data." << std::endl;
		container.saveLearnedData();
	}

	cleanUp();	
	return 0;
}
