
#ifndef SKELETONETHALON_H
#define SKELETONETHALON_H

#include <vector>

enum EthalonNode {
	HIPS,
		SPIN,
			SPIN_1,
				SPIN_2,
					NECK,
						HEAD,
					RIGHT_SHOULDER,
						RIGHT_ARM,
							RIGHT_ARM_ROLL,
								RIGHT_FORE_ARM,
									RIGHT_FORE_ARM_ROLL,
										RIGHT_HAND,
											RIGHT_HAND_THUMB_1,
												RIGHT_HAND_THUMB_2,
													RIGHT_HAND_THUMB_3,
														RIGHT_HAND_THUMB_4,
											RIGHT_HAND_INDEX_1,
												RIGHT_HAND_INDEX_2,
													RIGHT_HAND_INDEX_3,
														RIGHT_HAND_INDEX_4,
											RIGHT_HAND_MIDDLE_1,
												RIGHT_HAND_MIDDLE_2,
													RIGHT_HAND_MIDDLE_3,
														RIGHT_HAND_MIDDLE_4,
											RIGHT_HAND_RING_1,
												RIGHT_HAND_RING_2,
													RIGHT_HAND_RING_3,
														RIGHT_HAND_RING_4,
											RIGHT_HAND_PINKY_1,
												RIGHT_HAND_PINKY_2,
													RIGHT_HAND_PINKY_3,
														RIGHT_HAND_PINKY_4,
					LEFT_SHOULDER,
						LEFT_ARM,
							LEFT_ARM_ROLL,
								LEFT_FORE_ARM,
									LEFT_FORE_ARM_ROLL,
										LEFT_HAND,
											LEFT_HAND_THUMB_1,
												LEFT_HAND_THUMB_2,
													LEFT_HAND_THUMB_3,
														LEFT_HAND_THUMB_4,
											LEFT_HAND_INDEX_1,
												LEFT_HAND_INDEX_2,
													LEFT_HAND_INDEX_3,
														LEFT_HAND_INDEX_4,
											LEFT_HAND_MIDDLE_1,
												LEFT_HAND_MIDDLE_2,
													LEFT_HAND_MIDDLE_3,
														LEFT_HAND_MIDDLE_4,
											LEFT_HAND_RING_1,
												LEFT_HAND_RING_2,
													LEFT_HAND_RING_3,
														LEFT_HAND_RING_4,
											LEFT_HAND_PINKY_1,
												LEFT_HAND_PINKY_2,
													LEFT_HAND_PINKY_3,
														LEFT_HAND_PINKY_4,
		RIGHT_UP_LEG,
			RIGHT_UP_LEG_ROLL,
				RIGHT_LEG,
					RIGHT_LEG_ROLL,
						RIGHT_FOOT,
							RIGHT_TOE_BASE,
		LEFT_UP_LEG,
			LEFT_UP_LEG_ROLL,
				LEFT_LEG,
					LEFT_LEG_ROLL,
						LEFT_FOOT,
							LEFT_TOE_BASE,
};

class SkeletonEthalon {
public:
	SkeletonEthalon();
	~SkeletonEthalon();
	
	EthalonNode getRootNode();
	std::vector<EthalonNode> getChildNode(EthalonNode parent);
	bool isRequired(EthalonNode node);

private:

};

SkeletonEthalon::SkeletonEthalon() {
}

SkeletonEthalon::~SkeletonEthalon() {
}

EthalonNode SkeletonEthalon::getRootNode() {
	return HIPS;
}

std::vector<EthalonNode> SkeletonEthalon::getChildNode(EthalonNode parent) {
	std::vector<EthalonNode> children;
	switch (parent) {
	case HIPS:
		children.push_back(SPIN);
		children.push_back(RIGHT_UP_LEG);
		children.push_back(LEFT_UP_LEG);
		break;
	case SPIN:
		children.push_back(SPIN_1);
		break;
	case SPIN_1:
		children.push_back(SPIN_2);
		break;
	case SPIN_2:
		children.push_back(NECK);
		children.push_back(RIGHT_SHOULDER);
		children.push_back(LEFT_SHOULDER);
		break;
	case NECK:
		children.push_back(HEAD);
		break;
	case HEAD:
		break;
	case RIGHT_SHOULDER:
		children.push_back(RIGHT_ARM);
		break;
	case RIGHT_ARM:
		children.push_back(RIGHT_ARM_ROLL);
		break;
	case RIGHT_ARM_ROLL:
		children.push_back(RIGHT_FORE_ARM);
		break;
	case RIGHT_FORE_ARM:
		children.push_back(RIGHT_FORE_ARM_ROLL);
		break;
	case RIGHT_FORE_ARM_ROLL:
		children.push_back(RIGHT_HAND);
		break;
	case RIGHT_HAND:
		children.push_back(RIGHT_HAND_THUMB_1);
		children.push_back(RIGHT_HAND_INDEX_1);
		children.push_back(RIGHT_HAND_MIDDLE_1);
		children.push_back(RIGHT_HAND_RING_1);
		children.push_back(RIGHT_HAND_PINKY_1);
		break;
	case RIGHT_HAND_THUMB_1:
		children.push_back(RIGHT_HAND_THUMB_2);
		break;
	case RIGHT_HAND_THUMB_2:
		children.push_back(RIGHT_HAND_THUMB_3);
		break;
	case RIGHT_HAND_THUMB_3:
		children.push_back(RIGHT_HAND_THUMB_4);
		break;
	case RIGHT_HAND_THUMB_4:
		break;
	case RIGHT_HAND_INDEX_1:
		children.push_back(RIGHT_HAND_INDEX_2);
		break;
	case RIGHT_HAND_INDEX_2:
		children.push_back(RIGHT_HAND_INDEX_3);
		break;
	case RIGHT_HAND_INDEX_3:
		children.push_back(RIGHT_HAND_INDEX_4);
		break;
	case RIGHT_HAND_INDEX_4:
		break;
	case RIGHT_HAND_MIDDLE_1:
		children.push_back(RIGHT_HAND_MIDDLE_2);
		break;
	case RIGHT_HAND_MIDDLE_2:
		children.push_back(RIGHT_HAND_MIDDLE_3);
		break;
	case RIGHT_HAND_MIDDLE_3:
		children.push_back(RIGHT_HAND_MIDDLE_4);
		break;
	case RIGHT_HAND_MIDDLE_4:
		break;
	case RIGHT_HAND_RING_1:
		children.push_back(RIGHT_HAND_RING_2);
		break;
	case RIGHT_HAND_RING_2:
		children.push_back(RIGHT_HAND_RING_3);
		break;
	case RIGHT_HAND_RING_3:
		children.push_back(RIGHT_HAND_RING_4);
		break;
	case RIGHT_HAND_RING_4:
		break;
	case RIGHT_HAND_PINKY_1:
		children.push_back(RIGHT_HAND_PINKY_2);
		break;
	case RIGHT_HAND_PINKY_2:
		children.push_back(RIGHT_HAND_PINKY_3);
		break;
	case RIGHT_HAND_PINKY_3:
		children.push_back(RIGHT_HAND_PINKY_4);
		break;
	case RIGHT_HAND_PINKY_4:
		break;
	case LEFT_SHOULDER:
		children.push_back(LEFT_ARM);
		break;
	case LEFT_ARM:
		children.push_back(LEFT_ARM_ROLL);
		break;
	case LEFT_ARM_ROLL:
		children.push_back(LEFT_FORE_ARM);
		break;
	case LEFT_FORE_ARM:
		children.push_back(LEFT_FORE_ARM_ROLL);
		break;
	case LEFT_FORE_ARM_ROLL:
		children.push_back(LEFT_HAND);
		break;
	case LEFT_HAND:
		children.push_back(LEFT_HAND_THUMB_1);
		children.push_back(LEFT_HAND_INDEX_1);
		children.push_back(LEFT_HAND_MIDDLE_1);
		children.push_back(LEFT_HAND_RING_1);
		children.push_back(LEFT_HAND_PINKY_1);
		break;
	case LEFT_HAND_THUMB_1:
		children.push_back(LEFT_HAND_THUMB_2);
		break;
	case LEFT_HAND_THUMB_2:
		children.push_back(LEFT_HAND_THUMB_3);
		break;
	case LEFT_HAND_THUMB_3:
		children.push_back(LEFT_HAND_THUMB_4);
		break;
	case LEFT_HAND_THUMB_4:
		break;
	case LEFT_HAND_INDEX_1:
		children.push_back(LEFT_HAND_INDEX_2);
		break;
	case LEFT_HAND_INDEX_2:
		children.push_back(LEFT_HAND_INDEX_3);
		break;
	case LEFT_HAND_INDEX_3:
		children.push_back(LEFT_HAND_INDEX_4);
		break;
	case LEFT_HAND_INDEX_4:
		break;
	case LEFT_HAND_MIDDLE_1:
		children.push_back(LEFT_HAND_MIDDLE_2);
		break;
	case LEFT_HAND_MIDDLE_2:
		children.push_back(LEFT_HAND_MIDDLE_3);
		break;
	case LEFT_HAND_MIDDLE_3:
		children.push_back(LEFT_HAND_MIDDLE_4);
		break;
	case LEFT_HAND_MIDDLE_4:
		break;
	case LEFT_HAND_RING_1:
		children.push_back(LEFT_HAND_RING_2);
		break;
	case LEFT_HAND_RING_2:
		children.push_back(LEFT_HAND_RING_3);
		break;
	case LEFT_HAND_RING_3:
		children.push_back(LEFT_HAND_RING_4);
		break;
	case LEFT_HAND_RING_4:
		break;
	case LEFT_HAND_PINKY_1:
		children.push_back(LEFT_HAND_PINKY_2);
		break;
	case LEFT_HAND_PINKY_2:
		children.push_back(LEFT_HAND_PINKY_3);
		break;
	case LEFT_HAND_PINKY_3:
		children.push_back(LEFT_HAND_PINKY_4);
		break;
	case LEFT_HAND_PINKY_4:
		break;
	case RIGHT_UP_LEG:
		children.push_back(RIGHT_UP_LEG_ROLL);
		break;
	case RIGHT_UP_LEG_ROLL:
		children.push_back(RIGHT_LEG);
		break;
	case RIGHT_LEG:
		children.push_back(RIGHT_LEG_ROLL);
		break;
	case RIGHT_LEG_ROLL:
		children.push_back(RIGHT_FOOT);
		break;
	case RIGHT_FOOT:
		children.push_back(RIGHT_TOE_BASE);
		break;
	case RIGHT_TOE_BASE:
		break;
	case LEFT_UP_LEG:
		children.push_back(LEFT_UP_LEG_ROLL);
		break;
	case LEFT_UP_LEG_ROLL:
		children.push_back(LEFT_LEG);
		break;
	case LEFT_LEG:
		children.push_back(LEFT_LEG_ROLL);
		break;
	case LEFT_LEG_ROLL:
		children.push_back(LEFT_FOOT);
		break;
	case LEFT_FOOT:
		children.push_back(LEFT_TOE_BASE);
		break;
	case LEFT_TOE_BASE:
		break;
	default:
		break;
	}
	return children;
}

bool SkeletonEthalon::isRequired(EthalonNode node) {
	switch (node) {
	case HIPS:
	case SPIN:
	case SPIN_1:
	case SPIN_2:
	case NECK:
	case HEAD:
	case RIGHT_SHOULDER:
	case RIGHT_ARM:
	case RIGHT_ARM_ROLL:
	case RIGHT_FORE_ARM:
	case RIGHT_FORE_ARM_ROLL:
	case RIGHT_HAND:
		return true;
	case RIGHT_HAND_THUMB_1:
	case RIGHT_HAND_THUMB_2:
	case RIGHT_HAND_THUMB_3:
	case RIGHT_HAND_THUMB_4:
	case RIGHT_HAND_INDEX_1:
	case RIGHT_HAND_INDEX_2:
	case RIGHT_HAND_INDEX_3:
	case RIGHT_HAND_INDEX_4:
	case RIGHT_HAND_MIDDLE_1:
	case RIGHT_HAND_MIDDLE_2:
	case RIGHT_HAND_MIDDLE_3:
	case RIGHT_HAND_MIDDLE_4:
	case RIGHT_HAND_RING_1:
	case RIGHT_HAND_RING_2:
	case RIGHT_HAND_RING_3:
	case RIGHT_HAND_RING_4:
	case RIGHT_HAND_PINKY_1:
	case RIGHT_HAND_PINKY_2:
	case RIGHT_HAND_PINKY_3:
	case RIGHT_HAND_PINKY_4:
		return false;
	case LEFT_SHOULDER:
	case LEFT_ARM:
	case LEFT_ARM_ROLL:
	case LEFT_FORE_ARM:
	case LEFT_FORE_ARM_ROLL:
	case LEFT_HAND:
		return true;
	case LEFT_HAND_THUMB_1:
	case LEFT_HAND_THUMB_2:
	case LEFT_HAND_THUMB_3:
	case LEFT_HAND_THUMB_4:
	case LEFT_HAND_INDEX_1:
	case LEFT_HAND_INDEX_2:
	case LEFT_HAND_INDEX_3:
	case LEFT_HAND_INDEX_4:
	case LEFT_HAND_MIDDLE_1:
	case LEFT_HAND_MIDDLE_2:
	case LEFT_HAND_MIDDLE_3:
	case LEFT_HAND_MIDDLE_4:
	case LEFT_HAND_RING_1:
	case LEFT_HAND_RING_2:
	case LEFT_HAND_RING_3:
	case LEFT_HAND_RING_4:
	case LEFT_HAND_PINKY_1:
	case LEFT_HAND_PINKY_2:
	case LEFT_HAND_PINKY_3:
	case LEFT_HAND_PINKY_4:
		return false;
	case RIGHT_UP_LEG:
	case RIGHT_UP_LEG_ROLL:
	case RIGHT_LEG:
	case RIGHT_LEG_ROLL:
	case RIGHT_FOOT:
	case RIGHT_TOE_BASE:
	case LEFT_UP_LEG:
	case LEFT_UP_LEG_ROLL:
	case LEFT_LEG:
	case LEFT_LEG_ROLL:
	case LEFT_FOOT:
	case LEFT_TOE_BASE:
		return true;
	default:
		break;
		return false;
	}
}

#endif SKELETONETHALON_H