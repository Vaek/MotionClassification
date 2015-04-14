
#include "LearnDataContainer.h"
#include <cmath>
#include <queue>
#include "MotionClassRecognizer.h"
#include <fstream>
#include "LearnDataXmlHelper.h"

#define MIN_SIMILARITY 0
#define EXPORT_FILE_NAME "learned_data.xml"

LearnDataContainer::LearnDataContainer() : LearnDataContainer(EXPORT_FILE_NAME) { }

LearnDataContainer::LearnDataContainer(const std::string _fileName) : fileName(_fileName) { }

LearnDataContainer::~LearnDataContainer() { }

void LearnDataContainer::updateLearnMotion(std::string motionClass, MotionObject motionObject) {
	this->data.insert(std::pair<std::string, MotionObject>(motionClass, motionObject));
}

MotionFrame accumulateImportanceAndCreateAverageFrame(std::vector<MotionFrame>& commonFrames, std::map<std::string, double>& importances) {
	std::map<std::string, double> privateImportances;
	MotionFrame finalFrame = commonFrames.at(0);
	for (int i = 1; i < commonFrames.size(); i++) {
		auto frameA = commonFrames.at(i - 1);
		auto frameB = commonFrames.at(i);
		for each (auto statePair in frameA.getAllStates()) {
			auto state = statePair.second;
			if (frameB.hasMotionState(state.getName())) {
				double importance = 1.0 - MotionComparator::stateDifference(state, frameB.getMotionState(state.getName()));
				auto savedImportance = privateImportances.find(state.getName());
				if (savedImportance == privateImportances.end()) {
					privateImportances.insert(std::pair<std::string, double>(state.getName(), importance));
				} else {
					savedImportance->second += importance;
				}

			} else {
				if (privateImportances.find(state.getName()) == privateImportances.end()) {
					privateImportances.insert(std::pair<std::string, double>(state.getName(), 0));
				}
			}
		}
		finalFrame = finalFrame.averageWithFrame(frameB);
		// todo combine states to final frame
	}
	for each (auto importance in privateImportances) {
		auto tmp = importances.find(importance.first);
		if (tmp == importances.end()) {
			importances.insert(importance);
		} else {
			tmp->second += importance.second/(commonFrames.size()-1);
		}
	}
	return finalFrame;
}

void LearnDataContainer::combineAndUpdateLearnMotion(std::string motionClass, std::vector<MotionObject> keyframes) {
	int commonLength = INT_MAX;
	MotionObject combined;
	for each (auto mo in keyframes) {
		commonLength = std::min(commonLength, (int)mo.size());
	}

	std::map<std::string, double> importances;

	for (int i = 0; i < commonLength; i++) {
		std::vector<MotionFrame> commonFrames;
		for each (auto mo in keyframes) {
			commonFrames.push_back(mo.at(i));
		}
		combined.push_back(accumulateImportanceAndCreateAverageFrame(commonFrames, importances));
	}

	for each (auto importance in importances) combined.setNodeImportance(importance.first, importance.second / commonLength);

	this->updateLearnMotion(motionClass, combined);
}

MotionObject LearnDataContainer::getLearnMotionObject(std::string motionClass) {
	return this->data.find(motionClass)->second;
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

bool LearnDataContainer::loadLearnedData() {

	LearnDataXmlHelper helper;
	helper.parseXml(EXPORT_FILE_NAME);
	this->data = helper.readDocument();

	return true;
}