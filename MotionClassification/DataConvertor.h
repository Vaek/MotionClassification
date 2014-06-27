
#ifndef DATACONVERTOR_H
#define DATACONVERTOR_H

#include <fbxsdk.h>
#include <unordered_set>
#include <queue>
#include "Skeleton.h"
#include "Motion.h"

Skeleton* fbxToSkeleton(FbxScene* scene);

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

std::vector<FbxNode*> getAllSkeletonFbxNodes(FbxNode* root, std::vector<FbxNode*>& nodes) {
	std::queue<FbxNode*> queue;
	queue.push(root);
	while (!queue.empty()) {
		FbxNode* node = queue.back();
		queue.pop();
		for (int i = 0; i < node->GetChildCount(); i++) {
			queue.push(node->GetChild(i));
		}
		nodes.push_back(node);
	}

	return nodes;
}

Motion* fbxToMotion(FbxScene* scene, Skeleton* skeleton) {
	Motion* motion = new Motion();
	
	FbxNode* fbxRoot = scene->GetRootNode();

	if(fbxRoot) {
		for(int i = 0; i < fbxRoot->GetChildCount(); i++) {
			if (fbxRoot->GetChild(i)->GetName() == skeleton->getRoot()->getName()) {
				std::vector<FbxNode*> nodes;
				getAllSkeletonFbxNodes(fbxRoot->GetChild(i), nodes);
				FbxTimeSpan interval;
/*
				FbxAnimStack* stack = new FbxAnimStack(fbxRoot->GetFbxManager, fbxRoot->GetName());
				fbxRoot->GetChild(i)->GetAnimationInterval(interval, NULL, 0);
				for (int l = 0; l < stack->GetMemberCount(); l++) {
					FbxAnimLayer* layer = (FbxAnimLayer*)stack->GetMember(l);
					std::cout << layer->GetName() << '\t';
					FbxAnimCurve* curve = fbxRoot->GetChild(i)->LclTranslation.GetCurve(layer, false);
					if(curve) {
						std::cout << curve->KeyGetCount() << "curves\n";
						for(int k = 0; k<curve->KeyGetCount(); ++k) {
							FbxAnimCurveKey key = curve->KeyGet(k);
							float lKeyValue = key.GetValue();
							std::cout << key.GetTime().eFrames24 << " = " << lKeyValue << "\n";
						}
					}
				}
*/
				FbxAnimEvaluator* animEvaluator = fbxRoot->GetChild(i)->GetAnimationEvaluator();
				nodes[0]->GetAnimationInterval(interval, NULL, 0);
				for (int f = 0; f < interval.GetDuration().GetFieldCount(); f++){
					MotionFrame* frame = new MotionFrame();
					for (int n = 0; n < nodes.size(); n++) {
						animEvaluator->GetNodeLocalRotation(nodes[n], interval.GetStart(), nodes[n]->eSourcePivot, false, false);
					}					
				}
			}
		}
	}


	return motion;
}

#endif DATACONVERTOR_H