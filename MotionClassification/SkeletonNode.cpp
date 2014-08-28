
#include "SkeletonNode.h"

SkeletonNode::SkeletonNode(std::string name) : JoinBaseData(name) { }

SkeletonNode::~SkeletonNode() {
	for (SkeletonNodeMap::iterator it = children.begin(); it != this->children.end(); ++it) {
        delete it->second;
    }
    this->children.clear();
}

SkeletonNode* SkeletonNode::addChild(SkeletonNode* child) {
	std::pair<SkeletonNodeMap::iterator, bool> ret = this->children.insert(std::pair<std::string, SkeletonNode*>(child->getName(), child));
	if (ret.second==false) {
		return ret.first->second;
	} else {
		return NULL;
	}
}

SkeletonNodeMap& SkeletonNode::getChildren() {
	return this->children;
}
const SkeletonNodeMap& SkeletonNode::getChildren() const
{
	return this->children;
}
std::ostream& operator<<(std::ostream& out, const SkeletonNode& node) {
	out << "{" <<
		   "name : " << node.getName() << ",\n";
	const auto t = node.getTranslation();
	const auto r = node.getRotation();
	const auto s = node.getScaling();
	out << "translation : {" <<
				"x:" << t[0] << ", " << 
				"y:" << t[1] << ", " << 
				"z:" << t[2] << ", " << 
		   "},\n" <<
		   "rotation : {" << 
				"x:" << r[0] << ", " << 
				"y:" << r[1] << ", " << 
				"z:" << r[2] << ", " << 
		   "},\n" <<
		   "scaling : {" 
				"x:" << s[0] << ", " << 
				"y:" << s[1] << ", " << 
				"z:" << s[2] << ", " << 
		   "},\n";

	out << "children: [";
	for (const auto& child: node.getChildren()) {
		out << *child.second << ",\n";
	}

	out << "]\n}";
	return out;
}
