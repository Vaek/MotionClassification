
#ifndef FBXSCENEEXLOADER_H
#define FBXSCENEEXLOADER_H

#include <iostream>
#include <fbxsdk.h>

FbxScene* getScene(std::string filePath, FbxManager* manager);

#endif //FBXSCENEEXLOADER_H