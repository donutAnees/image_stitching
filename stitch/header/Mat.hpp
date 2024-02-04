#ifndef MAT_HPP
#define MAT_HPP

#include <string>

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
    ~Mat();
};

#endif