#ifndef STITCHER_HPP
#define STITCHER_HPP

#include <string>
#include <opencv2/core/mat.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

class Stitcher
{
public:
    std::vector<cv::Mat> &images;
    std::vector<cv::Mat> &grayscaledImages;
    cv::Mat &currentStitchedImage;
    std::vector<cv::KeyPoint> &currentImageKeypoints;
    cv::Mat &currentImageDescriptor;
    Stitcher(std::vector<cv::Mat>& images, std::vector<cv::Mat> &grayscaledImages, cv::Mat &currentStitchedImage,  std::vector<cv::KeyPoint> &currentImageKeypoints, cv::Mat &currentImageDescriptor);
    void processImage();
    void addImage(const std::string &filename);
};

#endif