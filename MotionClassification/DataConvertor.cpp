
#include "DataConvertor.h"

bool isFbxNodeTypeOf(FbxNode* fbxNode, FbxNodeAttribute::EType type) {
	for(int i = 0; i < fbxNode->GetNodeAttributeCount(); i++) {
		if (fbxNode->GetNodeAttributeByIndex(i)->GetAttributeType() == type) return true;
	}
	return false;
}

SkeletonNode* copyFbxNodeToSkeletonNode(FbxNode* fbxNode) {
	
	if (isFbxNodeTypeOf(fbxNode, FbxNodeAttribute::eSkeleton)) {
		SkeletonNode* skeletonNode = new SkeletonNode(fbxNode->GetName());
		
		FbxDouble3 translation = fbxNode->LclTranslation.Get(); 
		FbxDouble3 rotation = fbxNode->LclRotation.Get(); 
		FbxDouble3 scaling = fbxNode->LclScaling.Get();
		
		std::array<double, 3> translationArray = {translation.mData[0], translation.mData[1], translation.mData[2]};
		std::array<double, 3> rotationArray = {rotation.mData[0], rotation.mData[1], rotation.mData[2]};
		std::array<double, 3> scalingArray = {scaling.mData[0], scaling.mData[1], scaling.mData[2]};

		skeletonNode->setTranslation(translationArray)
					->setRotation(rotationArray)
					->setScaling(scalingArray);

		for(int i = 0; i < fbxNode->GetChildCount(); i++) {
			SkeletonNode* child = copyFbxNodeToSkeletonNode(fbxNode->GetChild(i));
			if (child) {
				skeletonNode->addChild(child);
			}
		}

		return skeletonNode;
	} else {
		return NULL;
	}
}

bool isAnnotatedNode(FbxNode* fbxNode, std::vector<node_info>& annotatedNodes) {
	for (auto annotatedNode : annotatedNodes) {
		if (annotatedNode.first->GetName() == fbxNode->GetName()) {
			return true;
		}
	}
	return false;
}

std::vector<SkeletonNode*> copyFbxNodeToSkeletonNode2(FbxNode* fbxNode, std::vector<node_info> annotatedNodes, std::vector<std::pair<std::string, std::string>> pathsNames) {
	std::vector<SkeletonNode*> nodes;

	std::string name;
	for (auto annotatedNode : annotatedNodes) {
		if (annotatedNode.first->GetName() == fbxNode->GetName()) {
			name = getAnnotatedName(pathsNames, annotatedNode.second);
			break;
		}
	}
	SkeletonNode* skeletonNode = nullptr;
	if (!name.empty()) {
		if (name == "None") name = fbxNode->GetName();
		skeletonNode = new SkeletonNode(name);

		FbxDouble3 translation = fbxNode->LclTranslation.Get();
		FbxDouble3 rotation = fbxNode->LclRotation.Get();
		FbxDouble3 scaling = fbxNode->LclScaling.Get();

		std::array<double, 3> translationArray = { translation.mData[0], translation.mData[1], translation.mData[2] };
		std::array<double, 3> rotationArray = { rotation.mData[0], rotation.mData[1], rotation.mData[2] };
		std::array<double, 3> scalingArray = { scaling.mData[0], scaling.mData[1], scaling.mData[2] };

		skeletonNode
			->setTranslation(translationArray)
			->setRotation(rotationArray)
			->setScaling(scalingArray);

		nodes.push_back(skeletonNode);
	}

	for(int i = 0; i < fbxNode->GetChildCount(); i++) {
		auto childFbxNode = fbxNode->GetChild(i);

		std::vector<SkeletonNode*> children = copyFbxNodeToSkeletonNode2(childFbxNode, annotatedNodes, pathsNames);
		if (skeletonNode) {
			for (SkeletonNode* child: children) {
				if (child) {
					skeletonNode->addChild(child);
				}
			}
		} else {
			for (SkeletonNode* child: children) {
				if (child) {
					nodes.push_back(child);
				}
			}
		}
		/*
		SkeletonNode* child = nullptr;

		for (auto annotatedNode : annotatedNodes) {
			if (annotatedNode.first->GetName() == childFbxNode->GetName()) {
				child = copyFbxNodeToSkeletonNode2(childFbxNode, annotatedNodes, pathsNames);
				break;
			}
		}
		child->getChildren
		if (child) {
			skeletonNode->addChild(child);
		}
		*/
	}

	return nodes;
}

std::vector<FbxNode*> getSkeletonFbxNodes(FbxNode* fbxNode) {
	std::vector<FbxNode*> nodes;
	if (isFbxNodeTypeOf(fbxNode, FbxNodeAttribute::eSkeleton)) {
		SkeletonNode* skeletonNode = new SkeletonNode(fbxNode->GetName());
		
		FbxDouble3 translation = fbxNode->LclTranslation.Get(); 
		FbxDouble3 rotation = fbxNode->LclRotation.Get(); 
		FbxDouble3 scaling = fbxNode->LclScaling.Get();
		
		std::array<double, 3> translationArray = {translation.mData[0], translation.mData[1], translation.mData[2]};
		std::array<double, 3> rotationArray = {rotation.mData[0], rotation.mData[1], rotation.mData[2]};
		std::array<double, 3> scalingArray = {scaling.mData[0], scaling.mData[1], scaling.mData[2]};

		skeletonNode->setTranslation(translationArray)
			->setRotation(rotationArray)
			->setScaling(scalingArray);

		for(int i = 0; i < fbxNode->GetChildCount(); i++) {
			SkeletonNode* child = copyFbxNodeToSkeletonNode(fbxNode->GetChild(i));
			if (child) {
				skeletonNode->addChild(child);
			}
		}
	}

	return nodes;
}

Skeleton* fbxToSkeleton(FbxScene* scene) {
	Skeleton* skeleton = new Skeleton();

	FbxNode* fbxRoot = scene->GetRootNode();
	if(fbxRoot) {
		for(int i = 0; i < fbxRoot->GetChildCount(); i++) {
			SkeletonNode* skeletonRoot = copyFbxNodeToSkeletonNode(fbxRoot->GetChild(i));

			if (skeletonRoot) {
				std::cout << "Found skeleton\n";
				if (skeleton->getRoot()) {
					std::cout << "Detected second skeleton " << fbxRoot->GetChild(i)->GetName() << ", stored only first." << std::endl;
					continue;
				}
				skeleton->setRoot(skeletonRoot);
			}
		}
	}

	return skeleton;
}

std::string getAnnotatedName(std::vector<std::pair<std::string, std::string>> pathsNames, std::string fbxPath) {
	for (auto pathName: pathsNames) {
		if (pathName.first == fbxPath) {
			return pathName.second;
		}
	}
	return "None";
}

Skeleton* fbxToSkeleton(FbxScene* scene, std::vector<node_info> annotatedNodes, std::vector<std::pair<std::string, std::string>> pathsNames) {
	Skeleton* skeleton = nullptr;
	
	auto skeletonRootNode = findRootSkeletonNode(scene->GetRootNode(), annotatedNodes);
	if (skeletonRootNode.first) {
		skeleton = new Skeleton();
		auto nodes = copyFbxNodeToSkeletonNode2(skeletonRootNode.first, annotatedNodes, pathsNames);
		if (nodes.size() > 0) {
			skeleton->setRoot(nodes.at(0));
		}
	}

	return skeleton;
}

std::vector<FbxNode*> getAllSkeletonFbxNodes(FbxNode* root, std::vector<FbxNode*>& nodes) {
	std::queue<FbxNode*> queue;
	queue.push(root);
	while (!queue.empty()) {
		auto node = queue.front();
		queue.pop();

		for (int i = 0; i < node->GetChildCount(); i++) {
			queue.push(node->GetChild(i));
		}
		nodes.push_back(node);
	}

	return nodes;
}

std::vector<node_info> getAllFbxNodesWithPaths(FbxNode* root) {
	std::queue<node_info> queue;
	std::vector<node_info> nodes;
	queue.push(std::make_pair(root,""));

	while (!queue.empty()) {
		auto node_pair = queue.front();
		queue.pop();

		auto node = node_pair.first;
		auto path = node_pair.second + "/" + node->GetName();

//		std::cout<< "Processing " << node->GetName() << "\n";
		for (int i = 0; i < node->GetChildCount(); i++) {
			auto child = node->GetChild(i);
//			std::cout<< "pushing " << child->GetName() << "\n";
			queue.push(std::make_pair(child,path));
		}
		nodes.push_back(std::make_pair(node,path));
	}

	return nodes;
}

std::pair<FbxNode*, std::string> findRootSkeletonNode(FbxNode* root, std::vector<node_info> annotatedNodes) {
	std::queue<node_info> queue;
	queue.push(std::make_pair(root,""));

	while (!queue.empty()) {
		auto node_pair = queue.front();
		queue.pop();

		auto node = node_pair.first;
		auto path = node_pair.second + "/" + node->GetName();

		for (int i = 0; i < node->GetChildCount(); i++) {
			auto child = node->GetChild(i);
			queue.push(std::make_pair(child, path));
		}

		for (auto annotatedNode : annotatedNodes) {
			if (annotatedNode.second == path) {
				return std::pair<FbxNode*, std::string>(node, path);
			}
		}
	}

	return std::pair<FbxNode*, std::string>(nullptr, "");
}

std::vector<node_info> findAnnotatedNodes(FbxNode* root, std::vector<std::pair<std::string, std::string>> annotatedNodes) {
	std::queue<node_info> queue;
	std::vector<node_info> nodes;

	queue.push(std::make_pair(root,""));

	while (!queue.empty()) {
		auto node_pair = queue.front();
		queue.pop();

		auto node = node_pair.first;
		auto path = node_pair.second + "/" + node->GetName();

		for (int i = 0; i < node->GetChildCount(); i++) {
			auto child = node->GetChild(i);
			queue.push(std::make_pair(child,path));
		}

		node_pair.second = path;
		for (auto annotatedPath : annotatedNodes) {
			if (annotatedPath.first == path) {
				nodes.push_back(node_pair);
			}
		}
	}
	std::vector<std::string> notFoundannotatedNodes;
	for (auto path: annotatedNodes) {
		bool found = false;
		for (auto nodeInfo: nodes) {
			if (nodeInfo.second == path.first) {
				found = true;
				break;
			}
		}
		if (!found) notFoundannotatedNodes.push_back(path.first);
	}
	if (!notFoundannotatedNodes.empty()) {
		std::cout << "Can not find these nodes:" << std::endl;
		for (auto path: notFoundannotatedNodes) {
			std::cout << path << std::endl;
		}
	}

	return nodes;
}

Motion* fbxToMotion(FbxScene* scene, Skeleton* skeleton) {
	Motion* motion = new Motion();
	
	FbxNode* fbxRoot = scene->GetRootNode();

	if(fbxRoot && skeleton->getRoot()) {
		for(int i = 0; i < fbxRoot->GetChildCount(); i++) {
			if (fbxRoot->GetChild(i)->GetName() == skeleton->getRoot()->getName()) {
				std::vector<FbxNode*> nodes;
				getAllSkeletonFbxNodes(fbxRoot->GetChild(i), nodes);

				FbxTimeSpan interval;
				fbxRoot->GetChild(i)->GetAnimationInterval(interval, NULL, 0);

//				FbxAnimLayer* animLayer = pAnimStack->GetMember<FbxAnimLayer>(0);

				for (unsigned int n = 0; n < nodes.size(); n++) {
					FbxNode* currentNode = nodes[n];
					AnimationCurve* curve = new AnimationCurve(currentNode->GetName());
					const long frameNumber = interval.GetDuration().GetFieldCount();
					curve->reserve(frameNumber);
/*
					AnimationCurve* transAnimCurve = currentNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_TRANSLATION);
					AnimationCurve* transAnimCurve = currentNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_ROTATION);
					AnimationCurve* transAnimCurve = currentNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_SCALING);
*/
					FbxTime currentTime = interval.GetStart();
					const auto timeMode = FbxTime::GetGlobalTimeMode();
					const auto timeStep = FbxTime::GetOneFrameValue(timeMode);
					for (long f = 0; f < frameNumber; f++){
						FbxVector4 rotation = currentNode->EvaluateLocalRotation(currentTime, currentNode->eSourcePivot, true, false);
						FbxVector4 translation = currentNode->EvaluateLocalTranslation(currentTime, currentNode->eSourcePivot, true, false);
						FbxVector4 scaling = currentNode->EvaluateLocalScaling(currentTime, currentNode->eSourcePivot, true, false);
//						curve->setTranslation(f, translation.Buffer());
//						curve->setRotation(f, rotation.Buffer());
//						curve->setScaling(f, scaling.Buffer());
						currentTime+=timeStep;
					}
					motion->addAnimationCurve(curve->getName(), curve);
				}
			}
		}
	}

	return motion;
}

std::vector<std::array<double, 3>> getTransformations(FbxNode* node, FbxAnimLayer* layer, Transformation transformation) {
	std::vector<std::array<double, 3>> transformations;
	
	FbxAnimCurve* curveX = nullptr;
	FbxAnimCurve* curveY = nullptr;
	FbxAnimCurve* curveZ = nullptr;

	switch (transformation) {
	case ROTATION:
		curveX = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X, false);
		curveY = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y, false);
		curveZ = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z, false);
		break;

	case TRANSLATION:
		curveX = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X, false);
		curveY = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y, false);
		curveZ = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z, false);
		break;

	case SCALING:
		curveX = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X, false);
		curveY = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y, false);
		curveZ = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z, false);
		break;

	default:
		break;
	}

	if (curveX && curveY && curveZ) {
		int minKeys = std::min(std::min(curveX->KeyGetCount(), curveY->KeyGetCount()), curveZ->KeyGetCount());
//		std::cout << minKeys << "\t" << curveX->KeyGetCount() << "\t" << curveY->KeyGetCount() << "\t" << curveZ->KeyGetCount() << node->GetName() << std::endl;
		/*
		if (minKeys<curveX->KeyGetCount() || minKeys<curveY->KeyGetCount() || minKeys<curveZ->KeyGetCount()) {
			std::cout << "reduce animation curve lenght " << node->GetName() << " to " << minKeys << std::endl;
		}
		*/
		transformations.resize(minKeys);
		for(int k = 0; k<minKeys; ++k) {
			FbxAnimCurveKey keyX = curveX->KeyGet(k);
			FbxAnimCurveKey keyY = curveY->KeyGet(k);
			FbxAnimCurveKey keyZ = curveZ->KeyGet(k);
			std::array<double, 3> rotation = {keyX.GetValue(), keyY.GetValue(), keyZ.GetValue()};
			transformations[k] = rotation;
//			float lKeyValue = keyX.GetValue();
//			std::cout << keyX.GetTime().Get() << ": " <<  lKeyValue << "\n";
		}
	}
	else {
//		std::cout << node->GetName() << " has not animation curve " << std::endl;
	}

	return transformations;
}

Motion* fbxToMotion(FbxScene* scene, std::vector<node_info> nodes, std::vector<std::pair<std::string, std::string>> pathsNames) {
	Motion* motion = nullptr;
	int numStacks = scene->GetSrcObjectCount<FbxAnimStack>();
	std::cout << "stacks: " << numStacks << "\n";
//	auto root = scene->GetRootNode();
	auto stack = scene->GetSrcObject<FbxAnimStack>(0);

//	std::vector<FbxNode*> nodes;
//	const auto nodes = getAllSkeletonFbxNodes2(root);
	std::cout << "nodes " <<  nodes.size() << std::endl;
	if (stack) {
		std::cout << "got stack" << std::endl;

		motion = new Motion();
		int animLayers = stack->GetMemberCount<FbxAnimLayer>();
		for (int l = 0; l < animLayers; l++) {
			FbxAnimLayer* layer = (FbxAnimLayer*)stack->GetMember(l);
			std::cout << layer->GetName() << std::endl;
			// iterate over nodes for which should be loaded animation
			for (auto&child_pair: nodes) {
				auto child=child_pair.first;
				auto name = getAnnotatedName(pathsNames, child_pair.second);
				if (name == "None") name = child->GetName();
				AnimationCurve* aCurve = new AnimationCurve(name);
//				std::cout << "\t" << child->GetName()<<": ";
				std::vector<std::array<double, 3>> rotations = getTransformations(child, layer, ROTATION);
				std::vector<std::array<double, 3>> translations = getTransformations(child, layer, TRANSLATION);
				std::vector<std::array<double, 3>> scalings = getTransformations(child, layer, SCALING);

				int frames = std::max(std::max(rotations.size(), translations.size()), scalings.size());
				aCurve->reserve(frames);

				for (int f = 0; f < frames; f++) {
					if (rotations.size() > f) aCurve->setRotation(f, rotations[f]);
					if (translations.size() > f) aCurve->setTranslation(f, translations[f]);
					if (scalings.size() > f) aCurve->setScaling(f, scalings[f]);
				}
				motion->addAnimationCurve(aCurve->getName(), aCurve);
			}
		}
	}
	return motion;
}
