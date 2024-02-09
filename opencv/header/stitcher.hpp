#ifndef STITCHER_HPP
#define STICHER_HPP

class Stitcher
{
public:
    std::vector<cv::Mat> &images;
    std::vector<cv::Mat> &grayscaledImages;
    cv::Mat &currentStitchedImage;
    std::vector<cv::KeyPoint> &currentImageKeypoints;
    cv::Mat &currentImageDescriptor;
    Stitcher();
    void processImage();
    void addImage(const std::string &filename);
};

#endif