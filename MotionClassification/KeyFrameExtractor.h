
#ifndef KEYFRAMEEXTRACTOR_H
#define KEYFRAMEEXTRACTOR_H

#include <fbxsdk.h>
#include <vector>
#include "KeyFrame.h"

std::vector<KeyFrame> extractKeyFrames(FbxScene* scene);

#endif //KEYFRAMEEXTRACTOR_H