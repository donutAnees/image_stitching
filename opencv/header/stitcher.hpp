#ifndef STITCHER_HPP
#define STITCHER_HPP

#include <string>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
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
    void mergeImage(cv::Mat& result,std::vector<cv::Point2f>& points1 ,std::vector<cv::Point2f>& points2, uint8_t middle);
    void Stitcher::setCurrentImage(cv::Mat& image);
};

#endif