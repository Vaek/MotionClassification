
#ifndef DATACONVERTOR_H
#define DATACONVERTOR_H

#include <fbxsdk.h>
#include <unordered_set>
#include <queue>
#include "Skeleton.h"
#include "Motion.h"

typedef std::pair<FbxNode*, std::string> node_info;

bool isFbxNodeTypeOf(FbxNode* fbxNode, FbxNodeAttribute::EType type);
SkeletonNode* copyFbxNodeToSkeletonNode(FbxNode* fbxNode);
std::vector<SkeletonNode*> copyFbxNodeToSkeletonNode2(FbxNode* fbxNode, std::vector<node_info> annotatedNodes, std::vector<std::pair<std::string, std::string>> pathsNames);
std::vector<FbxNode*> getSkeletonFbxNodes(FbxNode* fbxNode);

std::vector<FbxNode*> getAllSkeletonFbxNodes(FbxNode* root, std::vector<FbxNode*>& nodes);
std::vector<node_info> getAllFbxNodesWithPaths(FbxNode* root);
FbxNode* findRootSkeletonNode(FbxNode* root, std::vector<std::string> annotatedNodes);
std::pair<FbxNode*, std::string> findRootSkeletonNode(FbxNode* root, std::vector<node_info> annotatedNodes);
std::vector<node_info> findAnnotatedNodes(FbxNode* root, std::vector<std::pair<std::string, std::string>> annotatedNodes);

Skeleton* fbxToSkeleton(FbxScene* scene);
Skeleton* fbxToSkeleton(FbxScene* scene, std::vector<node_info> annotatedNodes, std::vector<std::pair<std::string, std::string>> pathsNames);

Motion* fbxToMotion(FbxScene* scene, Skeleton* skeleton);
Motion* fbxToMotion(FbxScene* scene, std::vector<node_info> annotatedNodes, std::vector<std::pair<std::string, std::string>> pathsNames);

std::string getAnnotatedName(std::vector<std::pair<std::string, std::string>> pathsNames, std::string fbxPath);

enum Transformation {
	ROTATION, TRANSLATION, SCALING
};

#endif //!DATACONVERTOR_H