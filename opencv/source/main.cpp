#include <iostream>
#include <vector>
#include "../header/stitcher.hpp"

using namespace std;

int main(int argc, char *argv[]){

    int count = 0;
    cin >> count;
    std::vector<cv::Mat> images;
    std::vector<cv::Mat> grayscaledImages;
    cv::Mat currentStitchedImages;
    std::vector<cv::KeyPoint> currentImageKeypoints;
    cv::Mat currentImageDescriptor;
    Stitcher stitcher = Stitcher(images,grayscaledImages,currentStitchedImages,currentImageKeypoints,currentImageDescriptor);

    for (int i = 0; i < count; i++){
        string temp;
        cin >> temp;
        stitcher.addImage(temp);
    }
    stitcher.processImage();
    return 0;
}