#include <iostream>
#include <vector>
#include <chrono>
#include "../header/stitcher.hpp"

using namespace std;

int main(int argc, char *argv[]){
    auto start = chrono::steady_clock::now();
    int count = 0; // the number of images
    bool flag; // flag for enabling and disabling resizing
    cin >> flag;
    cin >> count;
    // the following are the various attributes needed for the Stitcher Constructor
    std::vector<cv::Mat> images;
    std::vector<cv::Mat> grayscaledImages;
    cv::Mat currentStitchedImages , currentImageGrayscale;
    std::vector<std::vector<cv::KeyPoint>> imageKeypoints;
    std::vector<cv::Mat> imageDescriptors;
    cv::Mat currentImageDescriptor;
    std::vector<cv::KeyPoint> currentImageKeypoints;
    cv::Ptr<cv::Feature2D> orb = cv::ORB::create(10000); // Orb is used
    Stitcher stitcher = Stitcher(images,grayscaledImages,currentStitchedImages,imageDescriptors,imageKeypoints,currentImageDescriptor,orb,currentImageKeypoints,currentImageGrayscale);
    // the images are added to the Stitcher Object, which are then processed by it
    for (int i = 0; i < count; i++){
        string temp;
        cin >> temp;
        stitcher.addImage(temp,flag);
    }
    auto end = chrono::steady_clock::now(); 
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl; 
    // The images are processed and the result is written to result.jpg
    stitcher.processImage();
    end = chrono::steady_clock::now(); 
    duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl;
    return 0;
}