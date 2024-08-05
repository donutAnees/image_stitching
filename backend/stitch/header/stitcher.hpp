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
    private:
        // used to detect features
        static const cv::Ptr<cv::Feature2D> descriptor;
        // To get matches
        static const cv::BFMatcher matcher;
        // Vector of input images
        std::vector<cv::Mat> images;
        /* Features - Keypoints and Descriptor */
        // Vector of descriptors corresponding to each image
        std::vector<cv::Mat> imagesDescriptor;
        // Vector of keypoints corresponding to each image
        std::vector<std::vector<cv::KeyPoint>> imagesKeypoints;
        // Train Index and Query Index matches between previous and next image
        std::vector<std::pair<std::vector<int>, std::vector<int>>> matches;
        // Vector of cumulative homography from all the previous images to the next image
        std::vector<cv::Mat> homographies;
    public:
        /* Constructors */
        Stitcher();
        Stitcher(const cv::Mat &img);

        /* It converts the image to grayscale, detects keypoints and descriptors, and stores them along with the 
         * image in their respective containers. */
        void extract(const cv::Mat &img);

        /* This function calls extract and computes homography and storesthe matches. */
        void addImage(const cv::Mat &img);

        // Returns IDs of matched features given descriptors
        // {trainIds, queryIds}
        std::pair<std::vector<int>, std::vector<int>> getMatchingPoint(const cv::Mat &prev, const cv::Mat &current);
        
        // Get Rect of required Size for the output image
        cv::Rect warpRect(cv::Size sz, const cv::Mat &H);

        // Warp the image using homography
        cv::Mat warpImage(const cv::Mat &img, const cv::Mat &H, cv::Point orig);

        // Find the homography matrix
        cv::Mat getHomography( 
            const std::vector<int> &trainIDs, 
            const std::vector<int> &queryIDs, 
            const std::vector<cv::Point2f> &trainPts, 
            const std::vector<cv::Point2f> &queryPts
        );

        void StitchImages();
};      


#endif 