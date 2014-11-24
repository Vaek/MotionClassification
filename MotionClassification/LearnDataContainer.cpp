
#include "LearnDataContainer.h"
#include <cmath>
#include <queue>
#include "MotionClassRecognizer.h"
#include <fstream>

#define MIN_SIMILARITY 0
#define EXPORT_FILE_NAME "learned_data.mc"

LearnDataContainer::LearnDataContainer() { }

LearnDataContainer::~LearnDataContainer() { }

void LearnDataContainer::updateLearnMotion(std::string motionClass, std::vector<MotionFrame> keyFrames) {
	this->data.insert(std::pair<std::string, std::vector<MotionFrame>>(motionClass, keyFrames));
}

std::vector<MotionFrame> LearnDataContainer::getLearnMotionObject(std::string motionClass) {
	std::vector<MotionFrame> motionObject;
	return motionObject;
}

std::string LearnDataContainer::recognizeMotionClass(std::vector<MotionFrame> keyFrames) {
	std::vector<MotionClassRecognizer> recognizingQueue;

	for (auto learned : this->data) {
		recognizingQueue.push_back(MotionClassRecognizer(learned.first, learned.second, keyFrames));
	}

	bool goOn = false;
	int frame = 0;
	do {
		goOn = false;
		for (auto recognizer : recognizingQueue) {
			if (recognizer.compareFrame(frame)) goOn = true;
		}
		frame++;
	} while (goOn);
	
	std::string bestClass = "Unknown";
	double bestSimilarity = 0;
	for (int i=0; i<recognizingQueue.size(); i++) {
		auto recognizer = recognizingQueue.at(i);
		auto comparator = recognizer.getBestComparator();
		if (comparator.getSimilarity() > MIN_SIMILARITY && comparator.getSimilarity() > bestSimilarity) {
			bestClass = recognizer.getClassName();
			bestSimilarity = comparator.getSimilarity();
		}
	}

	return bestClass;
}

bool LearnDataContainer::saveLearnedData() {

	std::ofstream myfile;
	myfile.open(fileName);
	
	if (myfile.is_open() && scene) {
		// Print the nodes of the scene and their attributes recursively.
		// Note that we are not printing the root node because it should
		// not contain any attributes.
		FbxNode* lRootNode = scene->GetRootNode();
		if(lRootNode) {
			FbxAnimEvaluator* evaluator = lRootNode->GetAnimationEvaluator();
//			evaluator->GetNodeGlobalTransform();
			for(int i = 0; i < lRootNode->GetChildCount(); i++)
				saveNode(myfile, lRootNode->GetChild(i));
		}
	}
	myfile.close();


	return false;
}