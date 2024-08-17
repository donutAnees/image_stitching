#include <jni.h>
#include <iostream>
#include "com_example_imagestitching_service_JNIService.h"
#include "stitcher.hpp"

JNIEXPORT void JNICALL Java_com_example_imagestitching_service_JNIService_callStitcher
  (JNIEnv *env, jobject obj, jobject pathsList){
     // Get the ArrayList class
    jclass arrayListClass = env->GetObjectClass(pathsList);

    // Get the size method ID
    jmethodID sizeMethodID = env->GetMethodID(arrayListClass, "size", "()I");
    jint size = env->CallIntMethod(pathsList, sizeMethodID);

    // Get the get method ID
    jmethodID getMethodID = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");

    // Iterate over the ArrayList and retrieve each path
    std::vector<std::string> paths;
    for (jint i = 0; i < size; i++) {
        jobject pathObj = env->CallObjectMethod(pathsList, getMethodID, i);
        jclass stringClass = env->GetObjectClass(pathObj);
        jmethodID toStringMethodID = env->GetMethodID(stringClass, "toString", "()Ljava/lang/String;");
        jstring jstr = (jstring) env->CallObjectMethod(pathObj, toStringMethodID);
        const char *nativeString = env->GetStringUTFChars(jstr, 0);
        paths.push_back(nativeString);
        env->ReleaseStringUTFChars(jstr, nativeString);
    }

    stitchImagesFromPaths(paths);
  }