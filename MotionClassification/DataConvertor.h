
#ifndef DATACONVERTOR_H
#define DATACONVERTOR_H

#include <fbxsdk.h>
#include <unordered_set>
#include <queue>
#include "Skeleton.h"
#include "Motion.h"

bool isFbxNodeTypeOf(FbxNode* fbxNode, FbxNodeAttribute::EType type);
SkeletonNode* copyFbxNodeToSkeletonNode(FbxNode* fbxNode);
std::vector<FbxNode*> getSkeletonFbxNodes(FbxNode* fbxNode);

typedef std::pair<FbxNode*, std::string> node_info;
std::vector<FbxNode*> getAllSkeletonFbxNodes(FbxNode* root, std::vector<FbxNode*>& nodes);
std::vector<node_info> getAllFbxNodesWithPaths(FbxNode* root);
FbxNode* findRootSkeletonNode(FbxNode* root, std::vector<std::string> annotatedNodes);

Skeleton* fbxToSkeleton(FbxScene* scene);
Skeleton* fbxToSkeleton(FbxScene* scene, std::vector<std::string> annotatedNodes);

Motion* fbxToMotion(FbxScene* scene, Skeleton* skeleton);

#endif DATACONVERTOR_H