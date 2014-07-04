
#include "Skeleton.h"

Skeleton::Skeleton() : root(nullptr) {
}

Skeleton::~Skeleton() {
	delete root;
	this->root = nullptr;
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