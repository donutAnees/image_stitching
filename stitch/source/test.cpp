#include <iostream>
#include <fstream>

#include "../header/Mat.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    Mat mat = Mat("../img/img1.ppm");
    Mat gray = convertToGray(mat);
    // Write the grayscale image to a new PPM file
    writePPM(gray, "output_gray.ppm");
    return 0;
}