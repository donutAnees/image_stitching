#include <iostream>
#include <vector>
#include <chrono>
#include "../header/stitcher.hpp"

using namespace std;

int main(int argc, char *argv[]){
    auto start = chrono::steady_clock::now();
    int count = 0;
    cin >> count;
    std::vector<cv::Mat> images;
    std::vector<cv::Mat> grayscaledImages;
    cv::Mat currentStitchedImages;
    std::vector<std::vector<cv::KeyPoint>> imageKeypoints;
    std::vector<cv::Mat> imageDescriptors;
    cv::Mat currentImageDescriptor;
    std::vector<cv::KeyPoint> currentImageKeypoints;
    cv::Ptr<cv::Feature2D> orb = cv::ORB::create(10000);
    Stitcher stitcher = Stitcher(images,grayscaledImages,currentStitchedImages,imageDescriptors,imageKeypoints,currentImageDescriptor,orb,currentImageKeypoints);
    for (int i = 0; i < count; i++){
        string temp;
        cin >> temp;
        stitcher.addImage(temp,true);
    }
    auto end = chrono::steady_clock::now(); 
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl; 
    stitcher.processImage();
    end = chrono::steady_clock::now(); 
    duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl;
    return 0;
}