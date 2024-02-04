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
<<<<<<< HEAD
        int pixelValue;
        image >> pixelValue;
        Mat::data[i] = static_cast<unsigned char>(pixelValue);
=======
        image >> Mat::data[i];
>>>>>>> f7e197a70ee6eb9f6f9565116d2e706bd6dce495
    }
}

Mat::Mat(const Mat &image)
{
    rows = image.rows;
    cols = image.cols;
    channels = image.channels;
<<<<<<< HEAD
    size_t dataSize = rows * cols * 3;
    data = new unsigned char[dataSize];
    memcpy(data, image.data, sizeof(unsigned char) * dataSize);
}

Mat::Mat(int _rows, int _cols, int _channels, unsigned char *_data)
=======
    size_t dataSize = rows * cols * channels;
    data = new unsigned char[dataSize];
    memcpy(data, image.data, sizeof(image.data));
}

Mat::Mat(int _rows, int _cols, int _channels, unsigned char &_data)
>>>>>>> f7e197a70ee6eb9f6f9565116d2e706bd6dce495
{
    rows = _rows;
    cols = _cols;
    channels = _channels;
<<<<<<< HEAD
    size_t dataSize = rows * cols * 3;
    data = new unsigned char[dataSize];
    memcpy(data, _data, sizeof(unsigned char) * dataSize);
=======
    size_t dataSize = rows * cols * channels;
    data = new unsigned char[dataSize];
    memcpy(data, &_data, sizeof(unsigned char) * dataSize);
>>>>>>> f7e197a70ee6eb9f6f9565116d2e706bd6dce495
}

Mat::~Mat()
{
    delete[] Mat::data;
}

<<<<<<< HEAD
Mat convertToGray(Mat &image)
=======
Mat& Mat::convertToGray(Mat &image)
>>>>>>> f7e197a70ee6eb9f6f9565116d2e706bd6dce495
{
    int rows = image.rows;
    int cols = image.cols;
    size_t data_size = rows * cols * 3;
<<<<<<< HEAD
    unsigned char *newData = new unsigned char[data_size];
=======
    unsigned char *newData = new unsigned char[rows * cols];
    size_t counter = 0;
>>>>>>> f7e197a70ee6eb9f6f9565116d2e706bd6dce495
    for (int i = 0; i < data_size - 2; i += 3)
    {
        unsigned char r = image.data[i];
        unsigned char g = image.data[i + 1];
        unsigned char b = image.data[i + 2];
<<<<<<< HEAD
        newData[i] = 0.3 * r + 0.59 * g + 0.11 * b;
        newData[i + 1] = 0.3 * r + 0.59 * g + 0.11 * b;
        newData[i + 2] = 0.3 * r + 0.59 * g + 0.11 * b;
=======
        newData[counter++] = 0.3 * r + 0.59 * g + 0.11 * b;
>>>>>>> f7e197a70ee6eb9f6f9565116d2e706bd6dce495
    }
    Mat grayscaleImage = Mat(rows, cols, 1, newData);
    return grayscaleImage;
}

<<<<<<< HEAD
void writePPM(Mat &image , std::string& filename){
    std::ofstream outputFile;
    outputFile.open(filename);
    // Write the PPM header
    outputFile << "P3\n"
               << image.cols << " " << image.rows << "\n255\n";

    // Write the pixel data
    for (size_t i = 0; i < image.rows * image.cols * 3; ++i)
    {
        int pixelValue = image.data[i];                    // Get pixel value
        outputFile << static_cast<int>(pixelValue) << " "; // Write pixel value as binary data
    }

    outputFile.close();
}
=======
>>>>>>> f7e197a70ee6eb9f6f9565116d2e706bd6dce495
