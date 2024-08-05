package com.example.imagestitching.service;

public class JNIService {
    static {
        System.loadLibrary("libstitcher");
    }
}
