
#include "LearnDataContainer.h"
#include <cmath>
#include <queue>
#include "MotionClassRecognizer.h"
#include <fstream>
#include "LearnDataXmlHelper.h"

#define MIN_SIMILARITY 0
#define EXPORT_FILE_NAME "learned_data"

LearnDataContainer::LearnDataContainer() : LearnDataContainer(EXPORT_FILE_NAME) { }

LearnDataContainer::LearnDataContainer(const std::string fileName) : fileName(fileName) { }

LearnDataContainer::~LearnDataContainer() { }

void LearnDataContainer::updateLearnMotion(std::string motionClass, MotionObject motionObject) {
	this->data.insert(std::pair<std::string, MotionObject>(motionClass, motionObject));
}



std::vector<MotionFrame> LearnDataContainer::getLearnMotionObject(std::string motionClass) {
	MotionObject motionObject;
	return motionObject;
}

std::string LearnDataContainer::recognizeMotionClass(MotionObject motionObject) {
	std::vector<MotionClassRecognizer> recognizingQueue;

	for (auto learned : this->data) {
		recognizingQueue.push_back(MotionClassRecognizer(learned.first, learned.second, motionObject));
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

	LearnDataXmlHelper helper;
	helper.createDocument(this->data);
	helper.printXml(EXPORT_FILE_NAME);
	
	return true;
}