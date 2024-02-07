#include "../header/Mat.hpp"
#include <fstream>
#include <string>
#include <iostream>

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
        int pixelValue;
        image >> pixelValue;
        Mat::data[i] = static_cast<unsigned char>(pixelValue);
    }
}

Mat::Mat(const Mat &image)
{
    rows = image.rows;
    cols = image.cols;
    channels = image.channels;
    size_t dataSize = rows * cols * 3;
    data = new unsigned char[dataSize];
    memcpy(data, image.data, sizeof(unsigned char) * dataSize);
}

Mat::Mat(int _rows, int _cols, int _channels, unsigned char *_data)
{
    rows = _rows;
    cols = _cols;
    channels = _channels;
    size_t dataSize = rows * cols * 3;
    data = new unsigned char[dataSize];
    memcpy(data, _data, sizeof(unsigned char) * dataSize);
}

Mat::Mat() : rows(0), cols(0), channels(0), data(NULL) {}

Mat::~Mat()
{
    delete[] Mat::data;
}

Mat convertToGray(Mat &image)
{
    int rows = image.rows;
    int cols = image.cols;
    size_t data_size = rows * cols * 3;
    unsigned char *newData = new unsigned char[data_size];
    for (int i = 0; i < data_size - 2; i += 3)
    {
        unsigned char r = image.data[i];
        unsigned char g = image.data[i + 1];
        unsigned char b = image.data[i + 2];
        newData[i] = 0.3 * r + 0.59 * g + 0.11 * b;
        newData[i + 1] = 0.3 * r + 0.59 * g + 0.11 * b;
        newData[i + 2] = 0.3 * r + 0.59 * g + 0.11 * b;
    }
    Mat grayscaleImage = Mat(rows, cols, 1, newData);
    return grayscaleImage;
}

Mat resize(float factor, Mat &image)
{
    size_t cols = image.cols;
    size_t rows = image.rows;
    size_t newWidth = cols * factor;
    size_t newHeight = rows * factor;
    size_t newSize = newWidth * newHeight * 3;
    unsigned char *data = new unsigned char[newSize];
    size_t count = 0;
    for (int y = 0; y < newHeight; ++y)
    {
        for (int x = 0; x < (newWidth * 3); x += 3)
        {
            size_t originalY = y / factor;
            size_t originalX = x / factor;
            size_t index = originalY * (image.cols * 3) + originalX;
            data[count] = image.data[index];
            data[count+ 1] = image.data[index];
            data[count + 2] = image.data[index];
            count += 3;
        }
    }
    Mat resized = Mat(newHeight, newWidth, 1, data);
    return resized;
}

void writePPM(Mat &image, const std::string &filename)
{
    std::ofstream outputFile;
    outputFile.open(filename);
    // Write the PPM header
    outputFile << "P3\n"
               << image.cols << " " << image.rows << "\n255\n";

    // Write the pixel data
    for (size_t i = 0; i < image.rows * image.cols * 3; ++i)
    {
        int pixelValue = image.data[i];  // Get pixel value
        outputFile <<pixelValue << " "; // Write pixel value as binary data
    }

    outputFile.close();
}
