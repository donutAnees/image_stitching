#include "../header/Orb.hpp"
#include "../header/Feature.hpp"
#include "../header/Mat.hpp"
#include <fstream>
#include <cmath>

ORB::ORB(Mat &_image, std::vector<Keypoint> &_keypoints, Mat &_descriptor)
    : image(_image), keypoints(_keypoints), descriptor(_descriptor) {}

void ORB::detectKeypoints()
{
    int size = (image.rows * image.cols * 3);
    for (int i = 0; i < size - 2; i += 3)
    {
        // identify the intensity of the pixel of interest
        float I = image.data[i];
        // calculate the position of the pixels using the fact that position in 1D = (cols * y + x)
        float x = i % (image.cols * 3);
        float y = i / (image.cols * 3);
        if (3 <= x && x <= (image.cols*3 - 2) && 3 <= y && y <= (image.rows - 2))
        {
            std::pair<bool, unsigned char> keypointStatus = isPixelKeypoint(x, y, I);
            std::vector<Keypoint> allKeypoints;
            if (keypointStatus.first)
            {
                // append the detected pixel to the keypoints vector
                Keypoint keypoint = Keypoint(x, y , keypointStatus.second);
                allKeypoints.push_back(keypoint);
            }
            // perform Non Maximum Suppression to remove redundant and adjacent keypoints
            int radius = 3;
            for (size_t i = 0; i < allKeypoints.size()-1; ++i)
            {
                for (size_t j = i+1; j < allKeypoints.size(); ++i)
                {
                    if (std::pow((allKeypoints[i].pt.x - allKeypoints[j].pt.x),2) + std::pow((allKeypoints[i].pt.y - allKeypoints[j].pt.y),2) < std::pow(radius, 2))
                    {
                        if (allKeypoints[i].response > allKeypoints[j].response)
                        {
                            keypoints.push_back(allKeypoints[i]);
                        }
                        else if (allKeypoints[i].response == allKeypoints[j].response)
                        {
                            keypoints.push_back(allKeypoints[i]);
                            keypoints.push_back(allKeypoints[j]);
                        }
                        else
                        {
                            keypoints.push_back(allKeypoints[j]);
                        }
                    }
                }
            }
        } 
    }
}

void ORB::detectDescriptor()
{
}

std::pair<bool, unsigned char> ORB::isPixelKeypoint(int x, int y, float I)
{
    // Radius of the circle around the pixel = 3
    float threshold = 0.2 * I; // the intensity threshold to be considered an interest point
    // the pixels the circle touches are numbered from 1 to 16
    // to identify position of the pixel in a 1D matrix: (cols * y + x)
    // initially consider the pixels 1,5,9,13
    x = x + 3;
    int cols = image.cols * 3;
    unsigned char intensityAt1 = image.data[cols * (y - 3) + x];
    unsigned char intensityAt5 = image.data[cols * y + (x + 3)];
    unsigned char intensityAt9 = image.data[cols * (y + 3) + x];
    unsigned char intensityAt13 = image.data[cols * y + (x - 3)];
    // if atleast 3 of them are above or below the intensity (+/-) threshold then continue, else return false
    unsigned char count = 0;
    if (intensityAt1 > I + threshold || intensityAt1 < I - threshold)
    {
        count++;
    }

    if (intensityAt5 > I + threshold || intensityAt5 < I - threshold)
    {
        count++;
    }

    if (intensityAt9 > I + threshold || intensityAt9 < I - threshold)
    {
        count++;
    }

    if (intensityAt13 > I + threshold || intensityAt13 < I - threshold)
    {
        count++;
    }

    if (count >= 3)
    {
        // consider the other pixels, now that atleast 3 surpass the threshold intensity
        unsigned char intensityAt2 = image.data[cols * (y - 3) + (x + 1)];
        unsigned char intensityAt3 = image.data[cols * (y - 2) + (x + 2)];
        unsigned char intensityAt4 = image.data[cols * (y - 1) + (x + 3)];
        unsigned char intensityAt6 = image.data[cols * (y + 1) + (x + 3)];
        unsigned char intensityAt7 = image.data[cols * (y + 2) + (x + 2)];
        unsigned char intensityAt8 = image.data[cols * (y + 3) + (x + 1)];
        unsigned char intensityAt10 = image.data[cols * (y + 3) + (x - 1)];
        unsigned char intensityAt11 = image.data[cols * (y + 2) + (x - 2)];
        unsigned char intensityAt12 = image.data[cols * (y + 1) + (x - 3)];
        unsigned char intensityAt14 = image.data[cols * (y + 1) + (x - 3)];
        unsigned char intensityAt15 = image.data[cols * (y - 2) + (x - 2)];
        unsigned char intensityAt16 = image.data[cols * (y - 3) + (x - 1)];
        // if there are atleast 12 contiguous pixels that surpass the threshold, return true, else false

        std::vector<unsigned char> circleIntensity({intensityAt1, intensityAt2, intensityAt3, intensityAt4,
                                                    intensityAt5, intensityAt6, intensityAt7, intensityAt8,
                                                    intensityAt9, intensityAt10, intensityAt11, intensityAt12,
                                                    intensityAt13, intensityAt14, intensityAt15, intensityAt16});

        count = 0;
        for (unsigned char &a : circleIntensity)
        {
            if (a > I + threshold || a < I + threshold)
            {
                count++;
            }
            else
                count = 0;
        }
        if (count != 12)
            return {false, 0};
        // sum of Absolute difference between the intensity I and the 16 adjacent pixels
        unsigned char V = abs(I - intensityAt1) + abs(I - intensityAt2) + abs(I - intensityAt3) + abs(I - intensityAt4) +
                          abs(I - intensityAt5) + abs(I - intensityAt6) + abs(I - intensityAt7) + abs(I - intensityAt8) +
                          abs(I - intensityAt9) + abs(I - intensityAt10) + abs(I - intensityAt11) + abs(I - intensityAt12) +
                          abs(I - intensityAt13) + abs(I - intensityAt14) + abs(I - intensityAt15) + abs(I - intensityAt16);
        return {true, V};
    }
    else
        return {false, 0};
}
