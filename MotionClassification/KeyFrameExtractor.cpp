
#include "KeyFrameExtractor.h"
#include <map>
#include <stack>

FbxArray<FbxString*> mAnimStackNameArray;
FbxTime mFrameTime, mStart, mStop, mCurrentTime;
FbxTime mCache_Start, mCache_Stop;
std::map<std::string, FbxNode*> skeletonNodes;

bool getAnimStack(int pIndex, FbxScene* scene) {
	
	scene->FillAnimStackNameArray(mAnimStackNameArray);

    const int lAnimStackCount = mAnimStackNameArray.GetCount();
    if (!lAnimStackCount || pIndex >= lAnimStackCount)
    {
        return false;
    }

    // select the base layer from the animation stack
   FbxAnimStack * lCurrentAnimationStack = scene->FindMember<FbxAnimStack>(mAnimStackNameArray[pIndex]->Buffer());
   if (lCurrentAnimationStack == NULL)
   {
       // this is a problem. The anim stack should be found in the scene!
       return false;
   }

   // we assume that the first animation layer connected to the animation stack is the base layer
   // (this is the assumption made in the FBXSDK)
//   mCurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
   scene->SetCurrentAnimationStack(lCurrentAnimationStack);

   FbxTakeInfo* lCurrentTakeInfo = scene->GetTakeInfo(*(mAnimStackNameArray[pIndex]));
   if (lCurrentTakeInfo)
   {
       mStart = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
       mStop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
   }
   else
   {
       // Take the time line value
       FbxTimeSpan lTimeLineTimeSpan;
       scene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

       mStart = lTimeLineTimeSpan.GetStart();
       mStop  = lTimeLineTimeSpan.GetStop();
   }

   // check for smallest start with cache start
   if(mCache_Start < mStart)
	   mStart = mCache_Start;

   // check for biggest stop with cache stop
   if(mCache_Stop  > mStop)  
	   mStop  = mCache_Stop;

   // move to beginning
   mCurrentTime = mStart;

   return true;
}
// Get the global position of the node for the current pose.
// If the specified node is not part of the pose or no pose is specified, get its
// global position at the current time.
FbxAMatrix GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose, FbxAMatrix* pParentGlobalPosition)
{
    FbxAMatrix lGlobalPosition;
    bool        lPositionFound = false;

    if (pPose)
    {
        int lNodeIndex = pPose->Find(pNode);

        if (lNodeIndex > -1)
        {
            // The bind pose is always a global matrix.
            // If we have a rest pose, we need to check if it is
            // stored in global or local space.
            if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
            {
//                lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
            }
            else
            {
                // We have a local matrix, we need to convert it to
                // a global space matrix.
                FbxAMatrix lParentGlobalPosition;

                if (pParentGlobalPosition)
                {
                    lParentGlobalPosition = *pParentGlobalPosition;
                }
                else
                {
                    if (pNode->GetParent())
                    {
//                        lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
                    }
                }

//                FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
//                lGlobalPosition = lParentGlobalPosition * lLocalPosition;
            }

            lPositionFound = true;
        }
    }

    if (!lPositionFound)
    {
        // There is no pose entry for that node, get the current global position instead.

        // Ideally this would use parent global position and local position to compute the global position.
        // Unfortunately the equation 
        //    lGlobalPosition = pParentGlobalPosition * lLocalPosition
        // does not hold when inheritance type is other than "Parent" (RSrs).
        // To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
        lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
    }

    return lGlobalPosition;
}

// Get the geometry offset to a node. It is never inherited by the children.
FbxAMatrix GetGeometry(FbxNode* pNode)
{
    const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

    return FbxAMatrix(lT, lR, lS);
}

void DrawNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,
                       FbxAMatrix& pParentGlobalPosition, FbxPose* pPose)
{
    FbxAMatrix lGlobalPosition = GetGlobalPosition(pNode, pTime, pPose, &pParentGlobalPosition);

    if (pNode->GetNodeAttribute())
    {
    // Geometry offset.
    // it is not inherited by the children.
    FbxAMatrix lGeometryOffset = GetGeometry(pNode);
    FbxAMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

//        DrawNode(pNode, pTime, pAnimLayer, pParentGlobalPosition, lGlobalOffPosition, pPose);
    }

    const int lChildCount = pNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
    {
        DrawNodeRecursive(pNode->GetChild(lChildIndex), pTime, pAnimLayer, lGlobalPosition, pPose);
    }
}

void findAllSkeletonNodes(FbxScene* scene) {
	std::stack<FbxNode*> lastDiscoveredNodes;
	FbxNode* rootNode = scene->GetRootNode();
	for (int childIndex = 0; childIndex < rootNode->GetChildCount(); ++childIndex) {
		if (rootNode->GetChild(childIndex)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton) {
			lastDiscoveredNodes.push(rootNode->GetChild(childIndex));
			break;
		}
    }

	while (!lastDiscoveredNodes.empty()) {
		FbxNode* currentNode = lastDiscoveredNodes.top();
		lastDiscoveredNodes.pop();

		skeletonNodes.insert(std::pair<std::string, FbxNode*>(std::string(currentNode->GetName()), currentNode));
		
		for (int childIndex = 0; childIndex < currentNode->GetChildCount(); ++childIndex) {
			lastDiscoveredNodes.push(currentNode->GetChild(childIndex));
		}
	}	
}

std::vector<KeyFrame> extractKeyFrames(FbxScene* scene) {

	std::vector<KeyFrame> keyFrames;

	findAllSkeletonNodes(scene);

	if (getAnimStack(0, scene)) {
		for (FbxTime ct = mStart; ct < mStop; ct += ct.GetFramedTime()) {

		}
	}

	return keyFrames;
}