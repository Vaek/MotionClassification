
#ifndef FBXSCENEEXPORTER_H
#define FBXSCENEEXPORTER_H

#include <iostream>
#include <fstream>
#include <fbxsdk.h>


void exportToReadableFile(FbxScene* scene, std::string fileName);


#endif //FBXSCENEEXPORTER_H