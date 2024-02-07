#ifndef MAT_HPP
#define MAT_HPP

#include <string>
#include <vector>

class Mat
{
public:
    // the number of rows and columns
    size_t rows, cols;
    // the number of channels
    short channels;
    // the 3 channel image is stored in this array as linear but using the rows , columns and channels we can get into the the form [ [ [r,g,b] , [r,g,b] , [r,g,b] ] ,  [ [r,g,b] , [r,g,b] , [r,g,b] ] , ...]
    // each [r,g,b] is nothing but the individual pixel, and each [ [r,g,b] , [r,g,b] , [r,g,b] ] is the row of pixels
    unsigned char *data;
    Mat(const std::string &filepath);
    Mat(int _rows, int _cols, int _channels, unsigned char *_data);
    Mat(const Mat &image);
    Mat();
    // convert the 3 channels into 1 channel, by using luminosity method to get the grayscale value of the pixel, grayscale = .3 * R + .59 * G + .11 * B
    friend Mat convertToGray(Mat &image);
    friend Mat resize(float factor , Mat& image);
    friend void writePPM(Mat &image, const std::string &filename);
    ~Mat();
};

#endif