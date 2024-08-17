#include <jni.h>
#include <iostream>
#include "com_example_imagestitching_service_JNIService.h"
#include "stitcher.hpp"

JNIEXPORT void JNICALL Java_com_example_imagestitching_service_JNIService_callStitcher
  (JNIEnv *, jobject){
    std::vector<std::string> paths = {"/Users/anees/Desktop/im.png","/Users/anees/Desktop/im1.png"};
    stitchImagesFromPaths(paths);
  }