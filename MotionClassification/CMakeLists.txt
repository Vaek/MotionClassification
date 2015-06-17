#Find_Package(SDL REQUIRED)
#Find_Package(OPENNI2 REQUIRED)
Find_Package(OpenGL REQUIRED)
Find_Package(GLUT REQUIRED)
#pkg_check_modules(OpenCV REQUIRED opencv>=2.3.0)

add_executable( motion_classification
            AnimationCurve.cpp
            AnimationCurve.h
            AnnotationXmlHelper.cpp
            AnnotationXmlHelper.h
            DataConvertor.cpp
            DataConvertor.h
            FbxSceneExporter.cpp
            FbxSceneExporter.h
            FbxSceneLoader.cpp
            FbxSceneLoader.h
            FbxSceneXmlHelper.cpp
            FbxSceneXmlHelper.h
            JoinBaseData.cpp
            JoinBaseData.h
            KeyFrameExtractor.cpp
            KeyFrameExtractor.h
            LearnDataContainer.cpp
            LearnDataContainer.h
            LearnDataXmlHelper.cpp
            LearnDataXmlHelper.h
            Main.cpp
            MotionClassifier.cpp
            MotionClassifier.h
            MotionComparator.cpp
            MotionComparator.h
            Motion.cpp
            MotionFrame.cpp
            MotionFrame.h
            Motion.h
            MotionObject.cpp
            MotionObject.h
            MotionClassRecognizer.cpp
            MotionClassRecognizer.h
            MotionState.cpp
            MotionState.h
            MotionStateNode.h
            Scene.cpp
            Scene.h
            Skeleton.cpp
            SkeletonEthalon.h
            Skeleton.h
            SkeletonNode.cpp
            SkeletonNode.h
            Vector.h
            XmlHelper.cpp
            XmlHelper.h
            XmlNode.cpp
            XmlNode.h
            )
						
# PatternDetector.cpp PatternDetector.h						
include_directories ( 
			${OPENNI2_INCLUDE_DIRS}
			${SDL_INCLUDE_DIR} 
			${OPENGL_INCLUDE_DIRS}
			${GLUT_INCLUDE_DIRS})

target_link_libraries ( motion_classification 
			${OPENNI2_LIBRARIES} 
			${SDL_LIBRARY} 
			${OPENGL_LIBRARIES} 
			${OpenCV_LDFLAGS}
			${GLUT_LIBRARIES}
			)
			#GLEW 
			#jsoncpp)



