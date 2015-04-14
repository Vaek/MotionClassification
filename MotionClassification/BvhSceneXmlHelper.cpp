
#include "BvhSceneXmlHelper.h"
#include "BvhXmlNode.h"
#include <stack>

BvhSceneXmlHelper::BvhSceneXmlHelper() : AnnotationXmlHelper() {}

BvhSceneXmlHelper::~BvhSceneXmlHelper() {}

void BvhSceneXmlHelper::createDocument(const Bvh& scene, const std::string filePath) {
	std::stack<BvhXmlNode> stack;

	createDoctype();

	auto nodeScene = mXmlDocument.append_child(NODE_SCENE);
	nodeScene.append_attribute(ATTR_SOURCE_FBX) = filePath.c_str();

	stack.push(BvhXmlNode(nodeScene.append_child(NODE_STRUCTURE), scene.getRootJoint(), ""));

	while (!stack.empty()) {
		auto node = stack.top();
		stack.pop();

		for (auto child : node.createXmlNode()) {
			stack.push(child);
		}
	}
}
