
#include "FbxSceneXmlHelper.h"
#include "FbxXmlNode.h"
#include <stack>

FbxSceneXmlHelper::FbxSceneXmlHelper() : AnnotationXmlHelper() {}

FbxSceneXmlHelper::~FbxSceneXmlHelper() {}

void FbxSceneXmlHelper::createDocument(FbxScene* scene, const std::string filePath) {
	std::stack<FbxXmlNode> stack;

	createDoctype();

	auto nodeScene = mXmlDocument.append_child(NODE_SCENE);
	nodeScene.append_attribute(ATTR_SOURCE_FBX) = filePath.c_str();

	stack.push(FbxXmlNode(nodeScene.append_child(NODE_STRUCTURE), scene->GetRootNode(), ""));

	while (!stack.empty()) {
		auto node = stack.top();
		stack.pop();
		
		for (auto child : node.createXmlNode()) {
			stack.push(child);
		}
	}
}