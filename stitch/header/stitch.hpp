#ifndef STITCH_HPP
#define STITCH_HPP
#include "Mat.hpp"
#include "Feature.hpp"
#include <vector>
#include <string>

class BFMatcher
{
};

class Stitcher
{
    public:
    void drawKeypoints(Mat &image, std::vector<Keypoint> &keypoints,const std::string& filename);
};

#endif