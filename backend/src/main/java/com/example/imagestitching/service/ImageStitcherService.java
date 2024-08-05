package com.example.imagestitching.service;

import com.example.imagestitching.util.ImageStitcher;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import java.awt.*;

@Service
public class ImageStitcherService {
    @Autowired
    ImageStitcher imageStitcher;

    public Image getStitchedImage(MultipartFile[] images) {
        return imageStitcher.stitchImage(images);
    }
}