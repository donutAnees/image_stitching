package com.example.imagestitching.util;

import org.springframework.stereotype.Component;
import org.springframework.web.multipart.MultipartFile;

import java.awt.*;
import java.awt.image.BufferedImage;

@Component
public class ImageStitcher {
    @Autowired
    JNIService jniService;

    public Image stitchImage(MultipartFile[] images) {
         return jniService.callStitcher();
    }
}
