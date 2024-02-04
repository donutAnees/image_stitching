#include "../Header/Orb.hpp"

ORB::ORB(Mat &image, std::vector<Keypoint> &keypoints, Mat &descriptor)
    : image(image), keypoints(keypoints), descriptor(descriptor) {}

bool ORB::isPixelKeypoint(int x, int y, float I)
{
    // Radius of the circle around the pixel = 3
    float threshold = 0.2 * I; // the intensity threshold to be considered an interest point
    // the pixels the circle touches are numbered from 1 to 16
    // to identify position of the pixel in a 1D matrix: (cols * y + x) 
    // initially consider the pixels 1,5,9,13
    unsigned char intensityAt1 = image.data[image.cols * (y - 3) + x]; 
    unsigned char intensityAt5 = image.data[image.cols * y + (x + 3)];
    unsigned char intensityAt9 = image.data[image.cols * (y + 3) + x];
    unsigned char intensityAt13 = image.data[image.cols * y + (x - 3)]; 
    // if atleast 3 of them are above or below the intensity (+/-) threshold then continue, else return false
    int count = 0;
    if (intensityAt1 > I + threshold || intensityAt1 < I - threshold) count++;
    if (intensityAt5 > I + threshold || intensityAt5 < I - threshold) count++;
    if (intensityAt9 > I + threshold || intensityAt9 < I - threshold) count++;
    if (intensityAt13 > I + threshold || intensityAt13 < I - threshold) count++;
    if (count >= 3) {
        // consider the other pixels, now that atleast 3 surpass the threshold intensity
        unsigned char intensityAt2 = image.data[image.cols * (y - 3) + (x + 1)]; 
        unsigned char intensityAt3 = image.data[image.cols * (y - 2) + (x + 2)];
        unsigned char intensityAt4 = image.data[image.cols * (y - 1) + (x + 3)];
        unsigned char intensityAt6 = image.data[image.cols * (y + 1) + (x + 3)]; 
        unsigned char intensityAt7 = image.data[image.cols * (y + 2) + (x + 2)]; 
        unsigned char intensityAt8 = image.data[image.cols * (y + 3) + (x + 1)]; 
        unsigned char intensityAt10 = image.data[image.cols * (y + 3) + (x - 1)]; 
        unsigned char intensityAt11 = image.data[image.cols * (y + 2) + (x - 2)]; 
        unsigned char intensityAt12 = image.data[image.cols * (y + 1) + (x - 3)]; 
        unsigned char intensityAt14 = image.data[image.cols * (y + 1) + (x - 3)]; 
        unsigned char intensityAt15 = image.data[image.cols * (y - 2) + (x - 2)]; 
        unsigned char intensityAt16 = image.data[image.cols * (y - 3) + (x - 1)];
        // if there are atleast 12 contiguous pixels that surpass the threshold, return true, else false
        unsigned char circleIntensity[] = {intensityAt1, intensityAt2, intensityAt3, intensityAt4, 
        intensityAt5, intensityAt6, intensityAt7, intensityAt8, intensityAt9, intensityAt10, intensityAt11,
        intensityAt12, intensityAt13, intensityAt14, intensityAt15, intensityAt16};
        count = 0;
        for (int i = 0; i < 16; i++) {
            if (circleIntensity[i] > I + threshold || circleIntensity[i] < I + threshold) {
                count++;
                if (count == 12) return true;
            }
            else count = 0;
        }
        return false;
    }
    else return false;
}

void ORB::detectKeypoints()
{
    int size = (image.rows * image.cols * 3);
    for(int i = 0; i < size; i++){
        // identify the intensity of the pixel of interest
        float I = image.data[i];
        // calculate the position of the pixels using the fact that position in 1D = (cols * y + x) 
        int x = size % image.cols;
        int y = size / image.cols;
        if (isPixelKeypoint(x, y, I)){
            // append the detected pixel to the keypoints vector

        }
    }
}

void ORB::detectDescriptor()
{
}