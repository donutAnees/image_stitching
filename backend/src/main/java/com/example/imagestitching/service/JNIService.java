package com.example.imagestitching.service;

public class JNIService {
    static {
        System.loadLibrary("stitcher");
    }

    private native void callStitcher();
    
    public static void main(String args[]){
        JNIService demo = new JNIService();
        demo.callStitcher();
    }
}
