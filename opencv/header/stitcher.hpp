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
#include <numeric>
#include <iostream>

class Stitcher
{
public:
    std::vector<cv::Mat> &images;
    std::vector<cv::Mat> &imagesDescriptors;
    std::vector<std::vector<cv::KeyPoint>> &imagesKeypoints;
    std::vector<cv::Mat> &grayscaledImages;
    cv::Mat &currentStitchedImage;
    cv::Mat& currentImageDescriptor;
    std::vector<cv::KeyPoint>& currentImageKeypoints;
    cv::Ptr<cv::Feature2D>& orb;
    Stitcher(std::vector<cv::Mat>& images, std::vector<cv::Mat> &grayscaledImages, cv::Mat &currentStitchedImage,std::vector<cv::Mat>& imagesDescriptors ,std::vector<std::vector<cv::KeyPoint>> &imagesKeypoints,cv::Mat &currentImageDescriptor,cv::Ptr<cv::Feature2D>& orb,std::vector<cv::KeyPoint>& currentImageKeypoints);
    void processImage();
    void addImage(const std::string &filename,bool flag);
    void mergeMiddleImages(cv::Mat& result,std::vector<cv::Point2f>& points1 ,std::vector<cv::Point2f>& points2, uint8_t middle);
    void mergeMidRightImages(std::vector<cv::Point2f>& points1 ,std::vector<cv::Point2f>& points2, int leftIndex);
    void mergeLeftMidImages(std::vector<cv::Point2f>& points1 ,std::vector<cv::Point2f>& points2, int rightIndex);
    void setCurrentImage(cv::Mat& image);
    void showMatches(std::vector<cv::KeyPoint>& keypoints1, std::vector<cv::KeyPoint>& keypoints2,std::vector<cv::DMatch>& goodmatches,cv::Mat& images1, cv::Mat& images2);
    void mergeLeftMidRightImages(int leftIndex , int rightIndex);
    void getMatchingPoint(std::vector<cv::Point2f>& points1, std::vector<cv::Point2f>& points2,cv::Mat& descriptors1,cv::Mat& descriptors2, std::vector<cv::KeyPoint>& keypoints1, std::vector<cv::KeyPoint>& keypoints2,cv::Mat& image1,cv::Mat& image2);
    cv::Rect findWrapRect(cv::Size sz , cv::Mat& H);
    friend cv::Mat cropToNonBlackRegion(const cv::Mat& img);
    friend cv::Rect findNonBlackRegion(const cv::Mat& img);
    friend cv::Rect findLargestContourRect(const cv::Mat &img);
};

#endif