
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
		
		skeletonNode->setTranslation(translation.mData)
					->setRotation(rotation.mData)
					->setScaling(scaling.mData);

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

SkeletonNode* copyFbxNodeToSkeletonNode2(FbxNode* fbxNode) {
	SkeletonNode* skeletonNode = new SkeletonNode(fbxNode->GetName());
		
	FbxDouble3 translation = fbxNode->LclTranslation.Get(); 
	FbxDouble3 rotation = fbxNode->LclRotation.Get(); 
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
		
	skeletonNode->setTranslation(translation.mData)
				->setRotation(rotation.mData)
				->setScaling(scaling.mData);

	for(int i = 0; i < fbxNode->GetChildCount(); i++) {
		SkeletonNode* child = copyFbxNodeToSkeletonNode(fbxNode->GetChild(i));
		if (child) {
			skeletonNode->addChild(child);
		}
	}

	return skeletonNode;
}

std::vector<FbxNode*> getSkeletonFbxNodes(FbxNode* fbxNode) {
	std::vector<FbxNode*> nodes;
	if (isFbxNodeTypeOf(fbxNode, FbxNodeAttribute::eSkeleton)) {
		SkeletonNode* skeletonNode = new SkeletonNode(fbxNode->GetName());
		
		FbxDouble3 translation = fbxNode->LclTranslation.Get(); 
		FbxDouble3 rotation = fbxNode->LclRotation.Get(); 
		FbxDouble3 scaling = fbxNode->LclScaling.Get();
		
		skeletonNode->setTranslation(translation.mData)
			->setRotation(rotation.mData)
			->setScaling(scaling.mData);

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

Skeleton* fbxToSkeleton(FbxScene* scene, std::vector<std::string> annotatedNodes) {	
	Skeleton* skeleton = nullptr;
	
	FbxNode* skeletonRootNode = findRootSkeletonNode(scene->GetRootNode(), annotatedNodes);
	if (skeletonRootNode) {
		skeleton = new Skeleton();
		skeleton->setRoot(copyFbxNodeToSkeletonNode2(skeletonRootNode));
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

typedef std::pair<FbxNode*, std::string> node_info;
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

FbxNode* findRootSkeletonNode(FbxNode* root, std::vector<std::string> annotatedNodes) {
	std::queue<node_info> queue;
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

		for (auto annotatedPath : annotatedNodes) {
			if (annotatedPath == path) {
				return node;
			}
		}
	}

	return nullptr;
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

				for (int n = 0; n < nodes.size(); n++) {
					FbxNode* currentNode = nodes[n];
					AnimationCurve* curve = new AnimationCurve(currentNode->GetName());
					const int frameNumber = interval.GetDuration().GetFieldCount();
					curve->reserve(frameNumber);
/*
					AnimationCurve* transAnimCurve = currentNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_TRANSLATION);
					AnimationCurve* transAnimCurve = currentNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_ROTATION);
					AnimationCurve* transAnimCurve = currentNode->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_SCALING);
*/
					FbxTime currentTime = interval.GetStart();
					const auto timeMode = FbxTime::GetGlobalTimeMode();
					const auto timeStep = FbxTime::GetOneFrameValue(timeMode);
					for (int f = 0; f < frameNumber; f++){
						FbxVector4 rotation = currentNode->EvaluateLocalRotation(currentTime, currentNode->eSourcePivot, true, false);
						FbxVector4 translation = currentNode->EvaluateLocalTranslation(currentTime, currentNode->eSourcePivot, true, false);
						FbxVector4 scaling = currentNode->EvaluateLocalScaling(currentTime, currentNode->eSourcePivot, true, false);
						curve->setTranslation(f, translation.Buffer());
						curve->setRotation(f, rotation.Buffer());
						curve->setScaling(f, scaling.Buffer());
						currentTime+=timeStep;
					}
					motion->addMotionCurve(curve->getName(), curve);
				}
			}
		}
	}
/*	
	int numStacks = scene->GetSrcObjectCount<FbxAnimStack>();
	std::cout << "stacks: " << numStacks << "\n";
	auto root = scene->GetRootNode();
	auto stack = scene->GetSrcObject<FbxAnimStack>(0);

//	std::vector<FbxNode*> nodes;
	const auto nodes = getAllSkeletonFbxNodes2(root);
	std::cout << "nodes " <<  nodes.size()<<"\n";
	if (stack) {
		std::cout << "got stack\n";

		for (int l = 0; l < stack->GetMemberCount(); l++) {
			FbxAnimLayer* layer = (FbxAnimLayer*)stack->GetMember(l);
			std::cout << layer->GetName() << '\n';

			for (auto&child_pair: nodes) {

				auto child=child_pair.first;
//				std::cout << "\t" << child->GetName()<<": ";
				FbxAnimCurve* curve = child->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y,false);
				if (!curve) {
//					std::cout << "empty\n";
					continue;
				}
//				std::cout << "\t" << child_pair.second<<": " << curve->KeyGetCount() << " keyframes\n";

				if (child_pair.second == "/RootNode/Skeleton:Root/Skeleton:Reference/Skeleton:Hips/Skeleton:Spine_Dummy/Skeleton:Spine/Skeleton:Spine1") {
					for(int k = 0; k<curve->KeyGetCount(); ++k) {
						FbxAnimCurveKey key = curve->KeyGet(k);
						float lKeyValue = key.GetValue();
						std::cout << key.GetTime().Get() << ": " <<  lKeyValue << "\n";
					}
				}
			}
		}

	}
*/

	return motion;
}