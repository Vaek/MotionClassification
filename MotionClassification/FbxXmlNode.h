
#ifndef FBXXMLNODE_H
#define FBXXMLNODE_H

#include "XmlNode.h"
#include <fbxsdk.h>

class FbxXmlNode : public XmlNode {
public:
	FbxXmlNode::FbxXmlNode(pugi::xml_node _xmlNodeParent, FbxNode* _fbxNode, const std::string _fbxPath)
		: XmlNode(_xmlNodeParent), fbxNode(_fbxNode), fbxPath(_fbxPath) {};

	std::vector<FbxXmlNode> createXmlNode() {

		auto path = this->fbxPath + "/" + this->fbxNode->GetName();

		auto xmlChild = createChild(this->fbxNode->GetName(), path.c_str(), "None");

		std::vector<FbxXmlNode> children;
		//		std::cout<< "Processing " << node.fbxNode->GetName() << "\n";
		for (int i = 0; i < this->fbxNode->GetChildCount(); i++) {
			auto child = this->fbxNode->GetChild(i);
			//			std::cout << "pushing " << child->GetName() << "\n";
			children.push_back(FbxXmlNode(xmlChild, child, path));
		}
		return children;
	};

private:
	FbxNode* fbxNode;
	const std::string fbxPath;

};


#endif // FBXXMLNODE_H