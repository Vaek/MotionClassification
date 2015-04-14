
#ifndef BVHXMLNODE_H
#define BVHXMLNODE_H

#include "XmlNode.h"
#include "bvh\bvh.h"

class BvhXmlNode : public XmlNode {
public:
	BvhXmlNode::BvhXmlNode(pugi::xml_node _xmlNodeParent, const JOINT* _bvhNode, const std::string _bvhPath)
		: XmlNode(_xmlNodeParent), bvhNode(_bvhNode), bvhPath(_bvhPath) {};

	std::vector<BvhXmlNode> createXmlNode() {

		auto path = this->bvhPath + "/" + this->bvhNode->name;

		auto xmlChild = createChild(this->bvhNode->name, path.c_str(), "None");

		std::vector<BvhXmlNode> children;
		//		std::cout<< "Processing " << node.fbxNode->GetName() << "\n";
		for each (auto child in this->bvhNode->children) {
			//			std::cout << "pushing " << child->GetName() << "\n";
			children.push_back(BvhXmlNode(xmlChild, child, path));
		}
		return children;
	};

private:
	const JOINT* bvhNode;
	const std::string bvhPath;

};


#endif // BVHXMLNODE_H