
#ifndef KEYFRAMEEXTRACTOR_H
#define KEYFRAMEEXTRACTOR_H

#include <fbxsdk.h>
#include <vector>
#include "MotionFrame.h"
#include "Motion.h"
#include "MotionObject.h"

MotionObject extractKeyFrames(Motion* Motion);

#endif //KEYFRAMEEXTRACTOR_H