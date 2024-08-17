package com.example.imagestitching.service;
import java.util.ArrayList;

public class JNIService {
    static {
        System.loadLibrary("stitcher");
    }

    private native void callStitcher(ArrayList<String> paths);
    
    public static void main(String[] args){
        JNIService demo = new JNIService();
        ArrayList<String> paths = new ArrayList<>();
        paths.add("/Users/anees/Desktop/im.png");
        paths.add("/Users/anees/Desktop/im1.png");
        demo.callStitcher(paths);
    }
}
