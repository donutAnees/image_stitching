#ifndef STITCH_HPP
#define STICH_HPP

class stitch
{
public:
    std::vector<cv::Mat> &images;
    std::vector<cv::Mat> &grayscaledImages;
    cv::Mat &currentStitchedImage;
    std::vector<cv::KeyPoint> &currentImageKeypoints;
    cv::Mat &currentImageDescriptor;
    void processImage();
    void addImage(const std::string &filename);
};

#endif