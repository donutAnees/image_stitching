#ifndef STITCH_HPP
#define STITCH_HPP

#include <vector>
#include "Mat.hpp"

class Point2f
{
private:
    // x & y coordinates of the keypoint
    float x, y;

public:
    Point2f(float x, float y);
};

class Keypoint
{
private:
    Point2f pt;
    // size keypoint diameter
    // angle keypoint orientation
    // response keypoint detector response on the keypoint (that is, strength of the keypoint)
    // octave pyramid octave in which the keypoint has been detected
    float size;
    float angle;
    int octave;
    float response;

public:
    Keypoint(Point2f pt, float size, float angle = -1, int octave = 0, int response = 0);
};

class Features
{
private:
    std::vector<Keypoint> keypoints;
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