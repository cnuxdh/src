


MESSAGE(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR}) 
MESSAGE(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR}) 

SET(EXECUTABLE_OUTPUT_PATH ../)
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


#include and link lib path
INCLUDE_DIRECTORIES(/usr/local/include 
                    /Users/xiedonghai/Programs/Project/modules/CvLib)

LINK_DIRECTORIES(/usr/local/lib        
                 ../lib
                 /Users/xiedonghai/Programs/Project/build/modules/CvLib)


#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
#find_package(Eigen REQUIRED)
#include_directories(${EIGEN_INCLUDE_DIR})


#find_package(ceres REQUIRED)
#include_directories(${CERES_INCLUDE_DIRS})


#test program
#SET(SRC_LIST test.cpp)
#ADD_EXECUTABLE(hello ${SRC_LIST})

#ADD_EXECUTABLE(sample_hello helloworld.cc)
#TARGET_LINK_LIBRARIES(sample_hello ceres)


find_package( OpenCV REQUIRED )


#opencv based programs
ADD_EXECUTABLE(edge edge.cpp)
#TARGET_LINK_LIBRARIES(edge opencv_core opencv_highgui opencv_imgproc opencv_nonfree)
TARGET_LINK_LIBRARIES( edge ${OpenCV_LIBS} )



ADD_EXECUTABLE(smiledetect smiledetect.cpp)
TARGET_LINK_LIBRARIES(smiledetect opencv_core opencv_highgui opencv_objdetect opencv_imgproc opencv_nonfree)

ADD_EXECUTABLE(pano2plane pano2plane.cpp)
TARGET_LINK_LIBRARIES(pano2plane  ${OpenCV_LIBS} minpack lapack blas image f2c cblas matrix corelib cvlib)


#ADD_EXECUTABLE(testVoidPointer testVoidPointer.cpp)
ADD_EXECUTABLE(testOperatorOverride testOperatorOverride.cpp)

#ADD_EXECUTABLE(testCeres testCeres.cpp)
#TARGET_LINK_LIBRARIES(testCeres ceres)


#ADD_EXECUTABLE(DetectSift DetectSift.cpp)
#TARGET_LINK_LIBRARIES( DetectSift ${OpenCV_LIBS} )


ADD_EXECUTABLE(rp relativepose.cpp)
