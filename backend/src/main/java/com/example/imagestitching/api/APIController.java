package com.example.imagestitching.api;

import com.example.imagestitching.service.ImageStitcherService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.awt.*;

@RestController("/api")
@CrossOrigin(origins = "http://localhost:3000")
public class APIController {
    @Autowired
    ImageStitcherService imageStitcherService;

    @PostMapping(value = "/stitch-images", produces = MediaType.IMAGE_PNG_VALUE)
    public Image getStitchedImage(@RequestParam("files") MultipartFile[] images) {
        return imageStitcherService.getStitchedImage(images);
    }

    @GetMapping("/health")
    public ResponseEntity<String> healthCheck() {
        return ResponseEntity.ok("Server is up and runnning");
    }
}
