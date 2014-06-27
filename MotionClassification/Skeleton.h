#ifndef SKELETON_H
#define SKELETON_H

#include "SkeletonNode.h"

class Skeleton {
public:
	Skeleton();
	~Skeleton();
	
	Skeleton* setRoot(SkeletonNode* root);
	SkeletonNode* getRoot();
	
	friend std::ostream& operator<<(std::ostream& out, const Skeleton& node);
private:
	SkeletonNode* root;
};

Skeleton::Skeleton() {
}

Skeleton::~Skeleton() {
	delete root;
	this->root = NULL;
}

Skeleton* Skeleton::setRoot(SkeletonNode* root) {
	this->root = root;
	return this;
}

SkeletonNode* Skeleton::getRoot() {
	return this->root;
}

std::ostream& operator<< (std::ostream& out, Skeleton& node) {
	out << "{\"Skeleton\": " << *node.getRoot() << "}";
	return out;
}

#endif SKELETON_H