package com.example.imagestitching.service;
import org.springframework.stereotype.Component;

import java.util.ArrayList;

@Component
public class JNIService {
    static {
        System.loadLibrary("stitcher");
    }

    public native void callStitcher(ArrayList<String> paths);

    /* Uncomment if needed to just run this $ java -Djava.library.path=. JNIService.java */ 
    // public static void main(String[] args){
    //     JNIService demo = new JNIService();
    //     ArrayList<String> paths = new ArrayList<>();
    //     paths.add("/Users/anees/Desktop/im.png");
    //     paths.add("/Users/anees/Desktop/im1.png");
    //     demo.callStitcher(paths);
    // }
}
