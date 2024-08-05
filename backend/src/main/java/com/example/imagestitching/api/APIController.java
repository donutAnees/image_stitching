package com.example.imagestitching.api;

import com.example.imagestitching.service.ImageStitcherService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

import java.awt.*;

@RestController("/api")
public class APIController {
    @Autowired
    ImageStitcherService imageStitcherService;

    @PostMapping(value = "/stitch-images", produces = MediaType.IMAGE_PNG_VALUE)
    public Image getStitchedImage(@RequestParam MultipartFile[] images) {
        return imageStitcherService.getStitchedImage(images);
    }
}
