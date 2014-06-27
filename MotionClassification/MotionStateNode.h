
#ifndef MOTIONSTATENODE_H
#define MOTIONSTATENODE_H

#include <iostream>
#include <vector>
#include "KeyFrame.h"

struct MotionStateNode
{
	std::vector<MotionStateNode*> children; 
	KeyFrame state;
	std::string classification;

	~MotionStateNode() {
		for (int i = children.size()-1; i >= 0; i--) {
			delete this->children[i];
		}
	}
};

#endif //MOTIONSTATENODE_H