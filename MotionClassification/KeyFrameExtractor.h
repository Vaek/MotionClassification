
#ifndef KEYFRAMEEXTRACTOR_H
#define KEYFRAMEEXTRACTOR_H

#include <fbxsdk.h>
#include <vector>
#include "MotionFrame.h"
#include "Motion.h"

std::vector<MotionFrame> extractKeyFrames(Motion* Motion);

#endif //KEYFRAMEEXTRACTOR_H