#ifndef ORB_HPP
#define ORB_HPP

#include "Mat.hpp"
#include "Stitch.hpp"

class ORB
{
private:
    std::vector<Keypoint> &keypoints;
    Mat &image, &descriptor;

public:
    ORB(Mat &image, std::vector<Keypoint> &keypoints, Mat &descriptor);
    // Check if the given pixel coordinates and intensity (x, y, I) is a keypoint
    // Return true if it's a keypoint, otherwise false
    bool isPixelKeypoint(int x, int y, float I);
    // Perform keypoint detection algorithm using the image data
    // Store the detected keypoints in the 'keypoints' vector
    void detectKeypoints();
    // Compute descriptors for the detected keypoints
    // Store the descriptors in the 'descriptor' Mat object
    void detectDescriptor();
};

#endif