#ifndef SKELETON_H
#define SKELETON_H

#include "SkeletonNode.h"

class Skeleton {
public:
	Skeleton();
	~Skeleton();
	
	Skeleton* setRoot(SkeletonNode* root);
	SkeletonNode* getRoot();
	std::map<std::string, double> getNodeOfssets();
	
	friend std::ostream& operator<<(std::ostream& out, const Skeleton& node);
private:
	SkeletonNode* root;
};

#endif SKELETON_H