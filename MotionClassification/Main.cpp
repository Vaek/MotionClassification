
#include <iostream>
#include <fstream>

//#include <AntTweakBar.h>
#include <GL/glut.h>
// Math library glm
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <fbxsdk.h>
#include "FbxSceneLoader.h"
#include "FbxSceneExporter.h"
#include "DataConvertor.h"

const int DEFAULT_WINDOW_WIDTH = 720;
const int DEFAULT_WINDOW_HEIGHT = 486;

// Pointer to the tweak bar
//TwBar *bar;
// memory manager for FBX files
FbxManager* manager;

// 3D object zoom
float g_Zoom = 0.15f;

// 3D object orientation (stored as a quaternion)
glm::quat g_Rotation = glm::quat(0.06f, -0.69f, -0.64f, -0.33f);

// data
std::string dirPath = "./";
//const std::string fileName = "skeleton.fbx";
const std::string fileName = "pepe_mocap.fbx";
int showFrame;

// Clean up function called on end of program
static void cleanUp() {

	// Destroy the SDK manager and all the other objects it was handling.
	manager->Destroy();
}

// Callback function called by GLUT to render screen
void display(void) {

	// Clear frame buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);

	// Rotate and draw skeleton
	glPushMatrix();
	glMultMatrixf( glm::value_ptr( glm::mat4_cast(g_Rotation) ) );
	glScalef(g_Zoom, g_Zoom, g_Zoom);
	glPushMatrix();

	//	renderer->redisplaySkeleton();

	glCallList(1);
	glPopMatrix();

	glCallList(2);
	glPopMatrix();

	// Draw tweak bars
//	TwDraw();

	// Present frame buffer
	glutSwapBuffers();

	// Recall Display at next frame
	glutPostRedisplay();
}


// Callback function called by GLUT when window size changes
void reshape(int width, int height) {
	// Set OpenGL viewport and camera
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)width/height, 1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2, -1, 5, 
		0,  0, 0, 
		0,  1, 0);
	glTranslatef(0, 0.6f, -2);

	// Send the new window size to AntTweakBar
//	TwWindowSize(width, height);
}

void processSpecialKeys(int key, int xx, int yy) {
//	if (!TwEventKeyboardGLUT(key,xx,yy)) {
//		/*
//		endEffector->setPosition(key);
//		positionChanged = true;
//		*/
//	}
}

void processKeys(unsigned char key, int x, int y) {

//	if (!TwEventKeyboardGLUT(key,x,y)) {
//		switch (key) {
//		case TW_KEY_ESCAPE:
//			cleanUp();
//			exit(0);
//			break;
//		}
//	}
}

//void TW_CALL importFile(void *clientData) {
//	/*
//	skeleton = dataReader->readSkeleton(skeletonPath);
//	if (skeleton == NULL) {
//	std::cout << "Can not import skeleton." << endl;
//	} else {
//	renderer->setSkeleton(skeleton);
//	}
//	*/
//}

//static void createAntTweakBar() {
//
//	// Create a tweak bar
//	bar = TwNewBar("Controls");
//	TwDefine("Controls size='200 350' color='96 216 224' "); // change default tweak bar size and color
//
//	// Add 'g_Zoom' to 'bar': this is a modifable (RW) variable of type TW_TYPE_FLOAT. Its key shortcuts are [z] and [Z].
//	TwAddVarRW(bar, "Zoom", TW_TYPE_FLOAT, &g_Zoom,
//		" min=0.0001 max=2.5 step=0.001 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' ");
//
//	// Add 'g_Rotation' to 'bar': this is a variable of type TW_TYPE_QUAT4F which defines the object's orientation
//	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation,
//		" label='Object rotation' opened=true help='Change the object orientation.' ");
//
//	// Add 'g_Zoom' to 'bar': this is a modifable (RW) variable of type TW_TYPE_FLOAT. Its key shortcuts are [z] and [Z].
//	TwAddVarRW(bar, "File Path", TW_TYPE_STDSTRING, &fileName,
//		" help='Write whole path to file with data.' group='File' ");
//	// Add button to bar, after clicking adds a link to skeleton
//	TwAddButton(bar, "ImportSkeleton", importFile, NULL, " label='Import Skeleton' group='File' ");
//
//	TwAddVarRW(bar, "Show Frame", TW_TYPE_INT16, &showFrame,
//		" min=0 max=10000 step=1 help='Set number of frame you would like to see. [0=only skeleton]' group='Mocap'");
//
//}

static void initFbxManager() {

	// Initialize the SDK manager. This object handles all our memory management.
	manager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings *ioSettings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ioSettings);
}
using node_info = std::pair<FbxNode*, std::string>;
std::vector<node_info> getAllSkeletonFbxNodes2(FbxNode* root) {
	std::queue<node_info> queue;
	std::vector<node_info> nodes;
	queue.push({root,""});
	while (!queue.empty()) {

		auto node_pair = queue.front();
		auto node = node_pair.first;
		auto path = node_pair.second + "/" + node->GetName();

		queue.pop();
//		std::cout<< "Processing " << node->GetName() << "\n";
		for (int i = 0; i < node->GetChildCount(); i++) {
			auto child = node->GetChild(i);
//			std::cout<< "pushing " << child->GetName() << "\n";
			queue.push({child,path});
		}
		nodes.push_back({node,path});
	}

	return nodes;
}


// Main
int main(int argc, char** argv) {

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	glutCreateWindow("MotionClassification");
	glutCreateMenu(NULL);

	// Set GLUT callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	// Input methods
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	// Initialize AntTweakBar
//	TwInit(TW_OPENGL, NULL);
//
//	// Set GLUT event callbacks
//	// - Directly redirect GLUT mouse button events to AntTweakBar
//	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
//	// - Directly redirect GLUT mouse motion events to AntTweakBar
//	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
//	// - Directly redirect GLUT mouse "passive" motion events to AntTweakBar (same as MouseMotion)
//	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	// - Directly redirect GLUT key events to AntTweakBar
	// - Send 'glutGetModifers' function pointer to AntTweakBar;
	//   required because the GLUT key event functions do not report key modifiers states.
//	TwGLUTModifiersFunc(glutGetModifiers);
//
//	// create bar
//	createAntTweakBar();

	// init manager
	initFbxManager();

//	dirPath = "../../../data/fbx/";
//	fileName = "bone_skeleton.fbx";//"pepe_mocap.fbx";//"skeleton.fbx";
	
	// load data
	FbxScene* scene = getScene(dirPath + fileName, manager);

	int numStacks = scene->GetSrcObjectCount(FbxAnimStack::ClassId);
	std::cout << "stacks: " << numStacks << "\n";
	auto root = scene->GetRootNode();
	auto stack = FbxCast<FbxAnimStack>(scene->GetSrcObject(FbxAnimStack::ClassId, 0));
//	std::vector<FbxNode*> nodes;
	const auto nodes = getAllSkeletonFbxNodes2(root);
	std::cout << "nodes " <<  nodes.size()<<"\n";
	if (stack) {
		std::cout << "got stack\n";

		for (int l = 0; l < stack->GetMemberCount(); l++) {
			FbxAnimLayer* layer = (FbxAnimLayer*)stack->GetMember(l);
			std::cout << layer->GetName() << '\n';

			for (auto&child_pair: nodes) {

				auto child=child_pair.first;
//				std::cout << "\t" << child->GetName()<<": ";
				FbxAnimCurve* curve = child->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y,false);
				if (!curve) {
//					std::cout << "empty\n";
					continue;
				}
//				std::cout << "\t" << child_pair.second<<": " << curve->KeyGetCount() << " keyframes\n";

				if (child_pair.second == "/RootNode/Skeleton:Root/Skeleton:Reference/Skeleton:Hips/Skeleton:Spine_Dummy/Skeleton:Spine/Skeleton:Spine1") {
					for(int k = 0; k<curve->KeyGetCount(); ++k) {
						FbxAnimCurveKey key = curve->KeyGet(k);
						float lKeyValue = key.GetValue();
						std::cout << key.GetTime().Get() << ": " <<  lKeyValue << "\n";
					}
				}
			}
		}

	}

	for (auto&child_pair: nodes) {
		auto child = child_pair.first;
//		std::cout << child_pair.second << " is a " << child->GetTypeName();// << "\n";
//
		int attr_count = child->GetNodeAttributeCount();
		for (int a=0;a<attr_count;++a) {
//			std::cout << ", " << GetAttributeTypeName(child->GetNodeAttributeByIndex(a)->GetAttributeType());
		}
//		std::cout << "\n";
	}

	return 0;

	Skeleton* skeleton = fbxToSkeleton(scene);
	Motion* motion = nullptr;
	if (skeleton) {
		motion = fbxToMotion(scene, skeleton);
	}
	
	std::ofstream skeletonFile;
	skeletonFile.open("skeleton.txt");
	
	if (skeletonFile.is_open()) {
		skeletonFile << *skeleton->getRoot();
	}
	skeletonFile.close();
	
	if (motion) {
		std::ofstream motionFile;
		motionFile.open("motion.txt");
	
		if (motionFile.is_open()) {
			motionFile << *motion;
		}
		motionFile.close();
	}

	// export structure of scene
//	exportToReadableFile(scene, fileName + ".txt");
	
//	extractKeyFrames(scene);

	// Call the GLUT main loop
//	glutMainLoop();

	cleanUp();	

	return 0;
}
