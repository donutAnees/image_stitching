#include "../header/Stitch.hpp"
#include <fstream>
#include <iostream>


void Stitcher::drawKeypoints(Mat &_image, std::vector<Keypoint> &_keypoints,const std::string& filename)
{
    Mat keypointImage = Mat(_image);
    int rows = _image.rows;
    int cols = _image.cols;
    for (auto &_keypoint : _keypoints)
    {
        float x = _keypoint.pt.x;
        float y = _keypoint.pt.y;
        int pos = cols * y + x;
        keypointImage.data[pos * 3] = 255;
        keypointImage.data[pos * 3 + 1] = 0;
        keypointImage.data[pos * 3 + 2] = 0;
    }

    std::ofstream outputFile;
    outputFile.open(filename);
    outputFile << "P3\n"
               << cols << " " << rows << "\n255\n";

    // Write the pixel data
    for (size_t i = 0; i < rows * cols * 3; ++i)
    {
        int pixelValue = keypointImage.data[i]; // Get pixel value
        outputFile << pixelValue << " ";        // Write pixel value as binary data
    }
}