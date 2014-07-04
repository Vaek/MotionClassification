
#ifndef FBXSCENEEXPORTER_H
#define FBXSCENEEXPORTER_H

#include <iostream>
#include <fstream>
#include <fbxsdk.h>
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) ;

void exportToReadableFile(FbxScene* scene, std::string fileName);


#endif //FBXSCENEEXPORTER_H
