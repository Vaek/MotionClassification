
#include "LearnDataContainer.h"
#include <cmath>
#include <queue>
#include <fstream>
#include "LearnDataXmlHelper.h"
#include <iomanip>
#include <climits>

#define EXPORT_FILE_NAME "./learned/learned_data.xml"

LearnDataContainer::LearnDataContainer() : LearnDataContainer(EXPORT_FILE_NAME) { }

LearnDataContainer::LearnDataContainer(const std::string _fileName) : fileName(_fileName) { }

LearnDataContainer::~LearnDataContainer() { }

void LearnDataContainer::updateLearnMotion(std::string motionClass, MotionObject motionObject) {
	if (this->data.insert(std::pair<std::string, MotionObject>(motionClass, motionObject)).second == false) {
		this->data[motionClass] = motionObject;
	}
}

std::pair<std::map<std::string, double>, MotionFrame> computeImportanceAndCreateAverageFrame(std::vector<MotionFrame>& commonFrames) {
	std::map<std::string, double> privateImportances;
	MotionFrame finalFrame = commonFrames.at(0);
	for (int i = 1; i < commonFrames.size(); i++) {
		auto frameA = commonFrames.at(i - 1);
		auto frameB = commonFrames.at(i);
		for (auto statePair: frameA.getAllStates()) {
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

	std::map<std::string, double> finalImportances;
	for (auto importance: privateImportances) {
		finalImportances.insert(std::pair<std::string, double>(importance.first, importance.second / (commonFrames.size() - 1)));
	}
	return std::pair<std::map<std::string, double>, MotionFrame> (finalImportances, finalFrame);
}

void LearnDataContainer::combineAndUpdateLearnMotion(std::string motionClass, std::vector<MotionObject> keyframes) {
	int commonLength = INT_MAX;
	MotionObject combined;
	std::map<std::string, double> combinedOffsets;
	double maxNodeOffset = 0;
	for (auto mo: keyframes) {
		commonLength = std::min(commonLength, (int)mo.size());

		for (auto offsetPair: mo.getNodeOffsets()) {
			if (offsetPair.second > maxNodeOffset) {
				maxNodeOffset = offsetPair.second;
			}
			auto tmp = combinedOffsets.find(offsetPair.first);
			if (tmp == combinedOffsets.end()) {
				combinedOffsets.insert(offsetPair);
			}
			else {
				tmp->second += offsetPair.second;
			}
		}
	}

	for (auto offsetPair: combinedOffsets) {
		offsetPair.second = offsetPair.second / keyframes.size();
		combined.setNodeOffset(offsetPair);
	}

	std::map<std::string, double> importances;

	for (int i = 0; i < commonLength; i++) {
		std::vector<MotionFrame> commonFrames;
		for (auto mo: keyframes) {
			commonFrames.push_back(mo.at(i));
		}

		auto importanceFramePair = computeImportanceAndCreateAverageFrame(commonFrames);

		for (auto importance: importanceFramePair.first) {
			auto tmp = importances.find(importance.first);
			if (tmp == importances.end()) {
				importances.insert(importance);
			} else {
				tmp->second += importance.second;
			}
		}

		combined.push_back(importanceFramePair.second);
	}

	std::clog << "Most important joints are:" << "\n";
	for (auto importance: importances) {
		auto offsetQuatient = combined.getNodeOffset(importance.first) / maxNodeOffset;
		auto finalCombinedImportance = importance.second / commonLength * offsetQuatient;
		combined.setNodeImportance(importance.first, finalCombinedImportance);
		if (finalCombinedImportance > MotionObject::IMPORTANCE_LIMIT) {
			std::clog << "\t" << std::setw(17) << std::left << importance.first << finalCombinedImportance << "\n";
		}
	}

	this->updateLearnMotion(motionClass, combined);
}

MotionObject LearnDataContainer::getLearnMotionObject(std::string motionClass) {
	return this->data.find(motionClass)->second;
}
/*
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
		if (comparator.getSimilarity() > MotionClassRecognizer::RECOGNIZE_LIMIT && comparator.getSimilarity() > bestSimilarity) {
			bestClass = recognizer.getClassName();
			bestSimilarity = comparator.getSimilarity();
		}
	}

	return bestClass;
}
*/
std::vector<MotionClassRecognizer> LearnDataContainer::proccesRecognizing(MotionObject motionObject) {
	std::vector<MotionClassRecognizer> recognizingQueue;

	for (auto learned : this->data) {
		recognizingQueue.push_back(MotionClassRecognizer(learned.first, learned.second, motionObject));
	}

	for (auto recognizer : recognizingQueue) {
		recognizer.compareFrames();
	}

	std::clog << "\n";
	return recognizingQueue;
}


std::map<std::pair<long, long>, std::string> LearnDataContainer::recognizeMotionClass(MotionObject motionObject) {
//	auto proccesedRecognizers = proccesRecognizing(motionObject);
	std::vector<MotionClassRecognizer> proccesedRecognizers;

	for (auto learned : this->data) {
		auto recognizer = MotionClassRecognizer(learned.first, learned.second, motionObject);
		recognizer.compareFrames();
		proccesedRecognizers.push_back(recognizer);
		std::clog << "\n";
	}
	/*
	for (auto recognizer : proccesedRecognizers) {
		recognizer.compareFrames();
	}
	*/
	std::map<std::pair<long, long>, std::string> results;

	for (auto recognizer : proccesedRecognizers) {
		auto comparators = recognizer.getBestComparators();
		for (auto comparator: comparators) {
			if (comparator.getSimilarity() > MotionClassRecognizer::RECOGNIZE_LIMIT) {
				auto range = comparator.getRange();
				range.first = std::max(0L, range.first);
				range.second = std::min((long)motionObject.size(), range.second);

				results.insert(std::pair<std::pair<long, long>, std::string>(range, recognizer.getClassName()));
			}
		}
	}

	return results;
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
