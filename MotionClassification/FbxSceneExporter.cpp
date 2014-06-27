
#include "FbxSceneExporter.h"

/* Tab character ("\t") counter */
int numTabs = 0; 

/**
* Print the required number of tabs.
*/
void PrintTabs(std::ofstream& myfile) {
	for(int i = 0; i < numTabs; i++)
		myfile << "\t";
}

/**
* Return a string-based representation based on the attribute type.
*/
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) { 
	switch(type) { 
	case FbxNodeAttribute::eUnknown: return "unidentified"; 
	case FbxNodeAttribute::eNull: return "null"; 
	case FbxNodeAttribute::eMarker: return "marker"; 
	case FbxNodeAttribute::eSkeleton: return "skeleton"; 
	case FbxNodeAttribute::eMesh: return "mesh"; 
	case FbxNodeAttribute::eNurbs: return "nurbs"; 
	case FbxNodeAttribute::ePatch: return "patch"; 
	case FbxNodeAttribute::eCamera: return "camera"; 
	case FbxNodeAttribute::eCameraStereo: return "stereo"; 
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher"; 
	case FbxNodeAttribute::eLight: return "light"; 
	case FbxNodeAttribute::eOpticalReference: return "optical reference"; 
	case FbxNodeAttribute::eOpticalMarker: return "marker"; 
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve"; 
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface"; 
	case FbxNodeAttribute::eBoundary: return "boundary"; 
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface"; 
	case FbxNodeAttribute::eShape: return "shape"; 
	case FbxNodeAttribute::eLODGroup: return "lodgroup"; 
	case FbxNodeAttribute::eSubDiv: return "subdiv"; 
	default: return "unknown"; 
	} 
}

/**
* Print an attribute.
*/
void PrintAttribute(std::ofstream& myfile, FbxNodeAttribute* pAttribute) {
	if(!pAttribute) return;

	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	PrintTabs(myfile);
	// Note: to retrieve the character array of a FbxString, use its Buffer() method.
	myfile << "<attribute type=" << typeName.Buffer() << " name=" << attrName.Buffer() << "/>\n";
}

void saveNode(std::ofstream& myfile, FbxNode* pNode) {
	PrintTabs(myfile);
	const char* nodeName = pNode->GetName();
	FbxDouble3 translation = pNode->LclTranslation.Get(); 
	FbxDouble3 rotation = pNode->LclRotation.Get(); 
	FbxDouble3 scaling = pNode->LclScaling.Get();

	// Print the contents of the node.
	myfile << "<node name=" << nodeName << 
			" translation=(" << translation[0] << ", " << translation[1] << ", " << translation[2] << ")" << 
			" rotation=(" << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << ")" << 
			" scaling=(" << scaling[0] << ", " << scaling[1] << ", " << scaling[2] << ")>\n";
	numTabs++;

	// Print the node's attributes.
	for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		PrintAttribute(myfile, pNode->GetNodeAttributeByIndex(i));

	// Recursively print the children.
	for(int j = 0; j < pNode->GetChildCount(); j++)
		saveNode(myfile, pNode->GetChild(j));

	numTabs--;
	PrintTabs(myfile);
	myfile << "</node>\n";
}

void exportToReadableFile(FbxScene* scene, std::string fileName) {
	std::ofstream myfile;
	myfile.open(fileName);
	
	if (myfile.is_open() && scene) {
		// Print the nodes of the scene and their attributes recursively.
		// Note that we are not printing the root node because it should
		// not contain any attributes.
		FbxNode* lRootNode = scene->GetRootNode();
		if(lRootNode) {
			FbxAnimEvaluator* evaluator = lRootNode->GetAnimationEvaluator();
//			evaluator->GetNodeGlobalTransform();
			for(int i = 0; i < lRootNode->GetChildCount(); i++)
				saveNode(myfile, lRootNode->GetChild(i));
		}
	}
	myfile.close();
}
