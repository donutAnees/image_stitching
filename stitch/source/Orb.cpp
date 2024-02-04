#include "../Header/Orb.hpp"

ORB::ORB(Mat &image, std::vector<Keypoint> &keypoints, Mat &descriptor)
    : image(image), keypoints(keypoints), descriptor(descriptor) {}

bool ORB::isPixelKeypoint(int x, int y, int z)
{
    int radius = 3; // Radius of the circle around the pixel
    
}

void ORB::detectKeypoints()
{
}

void ORB::detectDescriptor()
{
}