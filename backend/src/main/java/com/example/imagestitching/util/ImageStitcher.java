package com.example.imagestitching.util;

import com.example.imagestitching.service.JNIService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.web.multipart.MultipartFile;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;

import static org.apache.tomcat.util.http.fileupload.FileUtils.deleteDirectory;

@Component
public class ImageStitcher {
    @Autowired
    JNIService jniService;

    BufferedImage outputImage;

    private String getFileExtension(String name) {
        String extension;
        try {
            extension = name.substring(name.lastIndexOf("."));
        } catch (Exception e) {
            extension = "";
        }
        return extension;
    }

    public Image stitchImage(MultipartFile[] images) {
        File imageSources = new File("../imagesources");
        boolean dirCreated = imageSources.mkdir();
        if(dirCreated) {
            int fileNum = 1;
            ArrayList<String> pathList = new ArrayList<>();
            for(MultipartFile image : images) {
                String name = image.getOriginalFilename();
                String fileName = "image"+fileNum;
                String fileExt = getFileExtension(name);
                String path = "../imagesources/"+fileName+"."+fileExt;
                pathList.add(path);
                try {
                    Path tempPath = Path.of(path);
                    image.transferTo(tempPath);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                fileNum++;
            }
            jniService.callStitcher(pathList);
            try {
                deleteDirectory(imageSources);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        File outputFile = new File("../service/source/result.jpg");
        try {
            outputImage = ImageIO.read(outputFile);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return outputImage;
    }
}
