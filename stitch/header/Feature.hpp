#ifndef FEATURE_HPP
#define FEATURE_HPP
#include "Mat.hpp"
#include <vector>

class Point2f
{
public:
    // x & y coordinates of the keypoint
    float x, y;
    Point2f() : x(0), y(0){};
    Point2f(float x, float y);
    Point2f(const Point2f &point);
};

class Keypoint
{
public:
    Point2f pt;
    // size keypoint diameter
    // angle keypoint orientation
    // response keypoint detector response on the keypoint (that is, strength of the keypoint)
    // octave pyramid octave in which the keypoint has been detectd
    float size;
    float angle;
    int octave;
    float response;
    Keypoint();
    Keypoint(const Keypoint &keypoint);
    Keypoint(Point2f &pt, float size, float angle, int octave, int response);
    Keypoint(float x, float y, unsigned char response);
};

class Features
{
public:
    std::vector<Keypoint> keypoints;
    Mat descriptor, image;
    Features(const Mat &image, std::vector<Keypoint> keypoints, Mat descriptor);
};

#endif