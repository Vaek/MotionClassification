
#include "FbxSceneLoader.h"

FbxScene* getScene(std::string filePath, FbxManager* manager) {

	// Create an importer using the SDK manager.
	FbxImporter* importer = FbxImporter::Create(manager,"");

	// Use the first argument as the filename for the importer.
	if(!importer->Initialize(filePath.c_str(), -1, manager->GetIOSettings())) { 
		printf("Call to FbxImporter::Initialize() failed.\n"); 
		printf("Error returned: %s\n\n", importer->GetStatus().GetErrorString()); 
		return nullptr;
	}

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* scene = FbxScene::Create(manager,"myScene");

	// Import the contents of the file into the scene.
	if (importer->Import(scene)) {
		std::cout << "Scene is succefully loaded." << std::endl;
	} else {
		std::cout << "Scene is NOT loaded." << std::endl;
	}

	// The file is imported; so get rid of the importer.
	importer->Destroy();

	return scene;
}