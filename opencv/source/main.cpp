#include <iostream>
#include <vector>
#include <opencv2/core/mat.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "../header/stitcher.hpp"

using namespace std;

int main(int argc, char *argv[]){
    if (argc < 3){
        cerr << "Usage: " << argv[0] << " <argument1> <argument2> [<argument3> <argument4>...]" <<endl;
        return -1;
    }
    std::vector<cv::Mat> images;
    std::vector<cv::Mat> grayscaledImages;
    cv::Mat currentStitchedImages;
    std::vector<cv::KeyPoint> currentImageKeypoints;
    cv::Mat currentImageDescriptor;
    Stitcher stitcher = Stitcher(images,grayscaledImages,currentStitchedImages,currentImageKeypoints,currentImageDescriptor);

    for (int i = 1; i < argc; i++){
        stitcher.addImage(argv[i]);
    }

    stitcher.processImage();

    return 0;
}