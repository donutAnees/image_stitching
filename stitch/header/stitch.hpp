#ifndef STITCH_HPP
#define STITCH_HPP
#include "Mat.hpp"
#include "Feature.hpp"
#include <vector>

class BFMatcher
{
};

class Stitcher
{
    public:
    friend void drawKeypoints(Mat &image, std::vector<Keypoint> &keypoints);
};

#endif