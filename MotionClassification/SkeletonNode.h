
#ifndef SKELETONNODE_H
#define SKELETONNODE_H

#include <map>
#include <vector>
#include <iostream>
#include "JoinBaseData.h"
#include <string>
class SkeletonNode;

typedef std::map<std::string, SkeletonNode*> SkeletonNodeMap;

class SkeletonNode : public JoinBaseData {
public:
	SkeletonNode(std::string name);
	~SkeletonNode();
	
	SkeletonNode* addChild(SkeletonNode* child);
	SkeletonNodeMap& getChildren();
	const SkeletonNodeMap& getChildren() const;

	friend std::ostream& operator<<(std::ostream& out, const SkeletonNode& node);
private:
	SkeletonNodeMap children;
};

#endif SKELETONNODE_H
