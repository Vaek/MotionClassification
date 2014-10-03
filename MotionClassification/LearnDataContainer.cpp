
#include "LearnDataContainer.h"
#include <cmath>

LearnDataContainer::LearnDataContainer() { }

LearnDataContainer::~LearnDataContainer() { }

void LearnDataContainer::updateLearnMotion(std::string motionClass, std::vector<MotionFrame> keyFrames) {
	this->data.insert(std::pair<std::string, std::vector<MotionFrame>>(motionClass, keyFrames));
}

std::vector<MotionFrame> LearnDataContainer::getLearnMotionObject(std::string motionClass) {
	std::vector<MotionFrame> motionObject;
	return motionObject;
}

double LearnDataContainer::compareFrames(MotionFrame learned, MotionFrame recognize) {
	double distance = 0;
	int numCompares = 0;
	for (auto learnedPair : learned.getAllStates()) {
		auto learnedState = learnedPair.second;
		try {
			auto recogState = recognize.getMotionState(learnedState.getName());

			double diffVectorSize = 0, learnedVectorSize = 0;
			for (int i = 0; i < 3; i++){
				diffVectorSize += std::pow(learnedState.getTranslation()[i] - recogState.getTranslation()[i], 2);
				learnedVectorSize += std::pow(learnedState.getTranslation()[i], 2);
			}
			diffVectorSize = std::sqrt(diffVectorSize);
			learnedVectorSize = std::sqrt(learnedVectorSize);

			if (learnedVectorSize > 0) distance += std::min(diffVectorSize/learnedVectorSize, 1.0);
			numCompares++;
		} catch (int e) { }
	}
	return 1.0-(distance/numCompares);
}

double LearnDataContainer::compareMotionObjects(std::vector<MotionFrame> learned, std::vector<MotionFrame> recognize) {
	double similarity = 0;
	int testedFrames = 0;
	for (unsigned int i=0; i<std::min(learned.size(), recognize.size()); i++) {
		testedFrames++;
		similarity += compareFrames(learned.at(i), recognize.at(i));
	}

	if (testedFrames == 0) {
		return 0;
	} else {
		return similarity/testedFrames;
	}	
}

std::string LearnDataContainer::recognizeMotionClass(std::vector<MotionFrame> keyFrames) {
	std::pair<std::string,std::vector<MotionFrame>> bestClass;
	bestClass.first = "Unknown";
	double best = 0;

	for (auto learned : this->data) {
		auto similarity = compareMotionObjects(learned.second, keyFrames);
		std::cout << "With " << learned.first << " is similar in " << similarity*100 << "%." << std::endl;
		if (similarity > best) {
			best = similarity;
			bestClass = learned;
		}
	}
	return bestClass.first;
}