
#include "Skeleton.h"
#include <queue>
#include <cmath>

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

std::map<std::string, double> Skeleton::getNodeOfssets() {
	std::queue<SkeletonNode*> queue;
	queue.push(root);
	std::map<std::string, double> offsets;
	while (!queue.empty()) {
		auto node = queue.front();
		queue.pop();

		for (auto child: node->getChildren()) {
			queue.push(child.second);
		}

		double offset = 0;
		if (node != root) {
			for (int i = 0; i < 3; i++) {
				offset += std::pow(node->getTranslation()[i], 2);
			}
		}

		offsets.insert(std::pair<std::string, double>(node->getName(), std::sqrt(offset)));
	}

	return offsets;
}

std::ostream& operator<< (std::ostream& out, Skeleton& node) {
	out << "{\"Skeleton\": " << *node.getRoot() << "}";
	return out;
}
