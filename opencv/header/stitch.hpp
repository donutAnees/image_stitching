#include <opencv2/core/mat.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

class stitch
{
public:
    std::vector<cv::Mat> &images;
    std::vector<cv::Mat> &grayscaledImages;
    cv::Mat &currentStitchedImage;
    std::vector<cv::KeyPoint> &currentImageKeypoints;
    cv::Mat &currentImageDescriptor;
    void processImage();
    void addImage();
};