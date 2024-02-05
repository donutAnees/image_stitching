#include "../header/Stitch.hpp"
#include <fstream>

void drawKeypoints(Mat &_image, std::vector<Keypoint> &_keypoints)
{
    Mat keypointImage = Mat(_image);
    int rows = _image.rows;
    int cols = _image.cols;
    for (auto &_keypoint : _keypoints)
    {
        float x = _keypoint.pt.x;
        float y = _keypoint.pt.y;
        int pos = cols * x + y;
        keypointImage.data[pos] = 255;
        keypointImage.data[pos + 1] = 0;
        keypointImage.data[pos + 2] = 0;
    }

    std::ofstream outputFile;
    outputFile.open("features.ppm");
    outputFile << "P3\n"
               << cols << " " << rows << "\n255\n";

    // Write the pixel data
    for (size_t i = 0; i < rows * cols * 3; ++i)
    {
        int pixelValue = keypointImage.data[i]; // Get pixel value
        outputFile << pixelValue << " ";        // Write pixel value as binary data
    }
}