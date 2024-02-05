#include "../header/Feature.hpp"
#include "../header/Mat.hpp"

Point2f::Point2f(float x, float y)
{
    this->x = x;
    this->y = y;
}

Point2f::Point2f(const Point2f &point)
{
    x = point.x;
    y = point.y;
}

Keypoint::Keypoint(const Keypoint &keypoint)
    : pt(keypoint.pt), size(keypoint.size), angle(keypoint.angle), octave(keypoint.octave), response(keypoint.response)
{
}

Keypoint::Keypoint(Point2f &pt, float size, float angle, int octave, int response)
    : pt(pt), size(size), angle(angle), octave(octave), response(response){};

Keypoint::Keypoint(float x, float y, unsigned char response)
    : pt(x, y), size(0), angle(-1), octave(0), response(response) {}

Features::Features(const Mat &image, std::vector<Keypoint> keypoints, Mat descriptor)
    : image(image), keypoints(keypoints), descriptor(descriptor)
{
}
