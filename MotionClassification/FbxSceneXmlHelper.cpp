
#include "FbxSceneXmlHelper.h"
#include "XmlNode.h"
#include <stack>

char* NODE_NODE = "node";
char* NODE_SCENE = "scene";
char* NODE_STRUCTURE = "structure";
char* NODE_ANNOTATION = "annotation";
char* ATTR_NAME = "name";
char* ATTR_PATH = "path";
char* ATTR_ANNOTATION = "annotation";
char* ATTR_SOURCE_FBX = "source_fbx";
char* ATTR_ANIM_LENGTH = "animationLength";

FbxSceneXmlHelper::FbxSceneXmlHelper() : XmlHelper() {}

FbxSceneXmlHelper::~FbxSceneXmlHelper() {}

class XmlFbxNode : public XmlNode {
public:
	XmlFbxNode::XmlFbxNode(pugi::xml_node _xmlNodeParent, FbxNode* _fbxNode, const std::string _fbxPath)
		: XmlNode(_xmlNodeParent), fbxNode(_fbxNode), fbxPath(_fbxPath) {};

	std::vector<XmlFbxNode> createXmlNode() {
		std::vector<XmlFbxNode> children;
		auto xmlChild = this->parent.append_child(NODE_NODE);

		auto path = this->fbxPath + "/" + this->fbxNode->GetName();

		xmlChild.append_attribute(ATTR_NAME) = this->fbxNode->GetName();
		xmlChild.append_attribute(ATTR_PATH) = path.c_str();
		xmlChild.append_attribute(ATTR_ANNOTATION) = "None";

		//		std::cout<< "Processing " << node.fbxNode->GetName() << "\n";
		for (int i = 0; i < this->fbxNode->GetChildCount(); i++) {
			auto child = this->fbxNode->GetChild(i);
			//			std::cout << "pushing " << child->GetName() << "\n";
			children.push_back(XmlFbxNode(xmlChild, child, path));
		}
		return children;
	};
	
private:
	FbxNode* fbxNode;
	const std::string fbxPath;
	
};

void FbxSceneXmlHelper::createDocument(FbxScene* scene, const std::string filePath) {
	std::stack<XmlFbxNode> stack;

	auto doctype = mXmlDocument.append_child(pugi::node_doctype);
	doctype.set_value("scene SYSTEM \"scene.dtd\"");

	auto nodeScene = mXmlDocument.append_child(NODE_SCENE);
	nodeScene.append_attribute(ATTR_SOURCE_FBX) = filePath.c_str();

	stack.push(XmlFbxNode(nodeScene.append_child(NODE_STRUCTURE), scene->GetRootNode(), ""));

	while (!stack.empty()) {
		auto node = stack.top();
		stack.pop();
		
		for (auto child : node.createXmlNode()) {
			stack.push(child);
		}
	}
}

pugi::xml_node FbxSceneXmlHelper::getAnnotation() {
	return mXmlDocument.child(NODE_SCENE).child(NODE_STRUCTURE);
}

std::vector<std::string> FbxSceneXmlHelper::getAnnotations() {
	std::vector<std::string> annotation;
	std::stack<pugi::xml_node> stack;

	stack.push(getAnnotation().child(NODE_NODE));

	while (!stack.empty()) {
		auto node = stack.top();
		stack.pop();

		annotation.push_back(node.attribute(ATTR_PATH).value());
		for (pugi::xml_node child = node.child(NODE_NODE); child; child = child.next_sibling(NODE_NODE)) {
			stack.push(child);
		}
	}

	return annotation;
}

std::string FbxSceneXmlHelper::getSourceFbx() {
	auto scene = mXmlDocument.child(NODE_SCENE);
	return scene.attribute(ATTR_SOURCE_FBX).value();
}