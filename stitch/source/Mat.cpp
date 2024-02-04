#include "../Header/Mat.hpp"

#include <fstream>
#include <string>

Mat::Mat(const std::string &filepath)
{
    std::ifstream image;
    image.open(filepath);

    // each ppm file has the following info in their header
    std::string type, width, height, Maxval;
    image >> type >> width >> height >> Maxval;

    Mat::rows = stoi(height);
    Mat::cols = stoi(width);

    // size needed to store the image
    size_t data_size = rows * cols * 3;
    Mat::data = new unsigned char[data_size]();

    Mat::channels = 3;

    // read till the end of the file and store each r,b,g as a single array
    for (size_t i = 0; i < data_size; ++i)
    {
        image >> Mat::data[i];
    }
}

Mat::Mat(const Mat& image) {
    rows = image.rows;
    cols = image.cols;
    channels = image.channels;
    size_t dataSize = rows * cols * channels;
    data = new unsigned char[dataSize];
    memcpy(data, image.data, sizeof(image.data));
}

Mat::~Mat()
{
    delete[] Mat::data;
}

Mat& Mat::convertToGray(Mat& image){
    Mat grayImage = Mat(image);
    size_t data_size = rows * cols * 3;
    channels = 1;
}