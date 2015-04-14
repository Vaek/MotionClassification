
#include "KeyFrameExtractor.h"
#include <map>
#include <stack>
#include <cmath>

#define MAX_TRANSLATION		100
#define MAX_ROTATION		100
#define MAX_SCALING			100

void acumulateTransformation(MotionFrame frame, double& translationSize) {
	for (auto state : frame.getAllStates()) {
		translationSize += MotionState::getSize(state.second.getTranslation());
	}
}

MotionFrame getMiddleFrame(std::vector<MotionFrame> frames) {
	return frames.at((int)std::floor(frames.size()/2));
}

MotionObject extractKeyFrames(Motion* motion) {
	MotionObject keyFrames;
	
	if (motion && motion->getMotionLength()>0) {
		std::vector<MotionFrame> partition;
		double acumulatedTranslation = 0;

		for (int i = 0; i < motion->getMotionLength(); i++){
			MotionFrame currentFrame = motion->getFrame(i);
			partition.push_back(currentFrame);

			acumulateTransformation(currentFrame, acumulatedTranslation);

			if (acumulatedTranslation > MAX_TRANSLATION) {
				keyFrames.push_back(getMiddleFrame(partition));
				acumulatedTranslation = 0;
				partition.clear();
			}
		}

		if (!partition.empty()) keyFrames.push_back(getMiddleFrame(partition));
	}

	return keyFrames;
}
