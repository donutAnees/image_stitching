#include <iostream>
#include <fstream>

#include "../Header/Mat.hpp"
using namespace std;

void writePPM(const Mat &image, const std::string &filename)
{
    std::ofstream outputFile;
    outputFile.open(filename);
    // Write the PPM header
    outputFile << "P3\n"
               << image.cols << " " << image.rows << "\n255\n";

    // Write the pixel data
for (size_t i = 0; i < image.rows * image.cols * 3; ++i)
    {
        int pixelValue = image.data[i]; // Get pixel value
        outputFile << static_cast<int>(pixelValue) << " "; // Write pixel value as binary data
    }
    outputFile.close();
}

int main(int argc, char *argv[]) {
    Mat mat = Mat("../img/img1.ppm");
    Mat gray = convertToGray(mat);
    // Write the grayscale image to a new PPM file
    writePPM(gray, "output_gray.ppm");
    return 0;
}