#ifndef STITCH_HPP
#define STITCH_HPP

#include <vector>
#include "Mat.hpp"

class Point2f
{
private:
    float x, y;

public:
    Point2f(float x, float y);
};

class Keypoints
{
};

class Descriptor
{
};

class Features
{
private:
    std::vector<Keypoints> keypoints;
    Mat descriptor;

public:
    Features(const Mat &image);
};

class ORB
{
};

class BFMatcher
{
};

class Stitcher
{
};

#endif