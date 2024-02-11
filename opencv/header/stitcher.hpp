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
    // Vector of input images
    std::vector<cv::Mat> &images;

    // Vector of descriptors corresponding to each image
    std::vector<cv::Mat> &imagesDescriptors;

    // Vector of keypoints corresponding to each image
    std::vector<std::vector<cv::KeyPoint>> &imagesKeypoints;

    // Vector of grayscale versions of input images
    std::vector<cv::Mat> &grayscaledImages;

    // Currently stitched image (result of stitching process)
    cv::Mat &currentStitchedImage;

    // Descriptor of the currently processed image
    cv::Mat &currentImageDescriptor;

    // Keypoints detected in the currently processed image
    std::vector<cv::KeyPoint> &currentImageKeypoints;

    // Feature detector and descriptor extractor
    cv::Ptr<cv::Feature2D> &orb;

    // constructor
    Stitcher(std::vector<cv::Mat> &images, std::vector<cv::Mat> &grayscaledImages, cv::Mat &currentStitchedImage, std::vector<cv::Mat> &imagesDescriptors, std::vector<std::vector<cv::KeyPoint>> &imagesKeypoints, cv::Mat &currentImageDescriptor, cv::Ptr<cv::Feature2D> &orb, std::vector<cv::KeyPoint> &currentImageKeypoints);

    // Process images to generate panorama
    void processImage();

    // Add image to the stitching pipeline
    void addImage(const std::string &filename, bool flag);

    // Merge the middle and middle + 1 images when the number of images are even
    void mergeMiddleImages(cv::Mat &result, std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, uint8_t middle);

    // Merge the current image with the image to its right
    void mergeMidRightImages(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, int rightIndex);

    // Merge the current image with the image to its left
    void mergeLeftMidImages(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, int leftIndex);

    // Merge current image with both left and right, this function uses the mergeMidRightImage and mergeLeftMidImage functions
    void mergeLeftMidRightImages(int leftIndex, int rightIndex);

    // Set the current image
    void setCurrentImage(cv::Mat &image);

    // Display matches between keypoints of two images (for debugging)
    void showMatches(std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2,
                     std::vector<cv::DMatch> &goodmatches, cv::Mat &image1, cv::Mat &image2);

    // Find matching keypoints between two images
    void getMatchingPoint(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2,
                          cv::Mat &descriptors1, cv::Mat &descriptors2,
                          std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2,
                          cv::Mat &image1, cv::Mat &image2);

    // Find the bounding rectangle after warp perspective transformation
    cv::Rect findWrapRect(cv::Size sz, cv::Mat &H);
    friend cv::Mat cropToNonBlackRegion(const cv::Mat &img);
    friend cv::Rect findNonBlackRegion(const cv::Mat &img);
    friend cv::Rect findLargestContourRect(const cv::Mat &img);
};

#endif