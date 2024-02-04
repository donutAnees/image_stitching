#include "../Header/Orb.hpp"

ORB::ORB(Mat &image, std::vector<Keypoint> &keypoints, Mat &descriptor)
    : image(image), keypoints(keypoints), descriptor(descriptor) {}

bool ORB::isPixelKeypoint(int x, int y, int z)
{
    unsigned char radius = 3; // Radius of the circle around the pixel
    // the pixels the circle touches are numbered from 1 to 16
    // initially consider the pixels 1,5,9,13, if atleast 3 of them are above or below the intesity (+/-) threshold then continue, else return false
    unsigned char intensityAt1 = ;
    unsigned char intensityAt5 = ;
    unsigned char intensityAt9 = ;
    unsigned char intensityAt13 = ; 

}

void ORB::detectKeypoints()
{
}

void ORB::detectDescriptor()
{
}