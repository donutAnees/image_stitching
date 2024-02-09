#include "../header/stitcher.hpp"

Stitcher::Stitcher(std::vector<cv::Mat>& images, std::vector<cv::Mat> &grayscaledImages, cv::Mat &currentStitchedImage,  std::vector<cv::KeyPoint> &currentImageKeypoints, cv::Mat &currentImageDescriptor)
        : images(images), grayscaledImages(grayscaledImages),
          currentStitchedImage(currentStitchedImage),
          currentImageKeypoints(currentImageKeypoints),
          currentImageDescriptor(currentImageDescriptor) {}  
          
void Stitcher::processImage()
{
    uint8_t imageCount = images.size();
    uint8_t middle = imageCount / 2 - 1;
    cv::Ptr<cv::Feature2D> orb = cv::ORB::create(10000);
    if (imageCount % 2 == 0){
        cv::Mat descriptors1, descriptors2;
        std::vector<cv::KeyPoint> keypoints1, keypoints2;
        std::vector<bool> states1;
        std::vector<bool> states2;
        orb->detectAndCompute(images[middle], states1, keypoints1, descriptors1);
        orb->detectAndCompute(images[middle + 1], states2, keypoints2, descriptors2);
        std::vector<std::vector<cv::DMatch>> matches;
        cv::BFMatcher matcher = cv::BFMatcher(cv::NORM_HAMMING2); // Hamming is good for binary string based algorithms , cross checker can be true if ratio is not used
        matcher.knnMatch(descriptors1, descriptors2, matches, 2);
        std::vector<cv::DMatch> goodmatches;
        for (int i = 0; i < matches.size(); i++)
        {
            if (matches[i][0].distance < 0.75 * matches[i][1].distance)
            {
                goodmatches.emplace_back(matches[i][0]);
            }
        }
        cv::Mat imMatches;
        cv::drawMatches(images[middle], keypoints1, images[middle+1], keypoints2, goodmatches, imMatches);
        cv::imshow("result", imMatches);
        cv::waitKey(0);
        std::vector<cv::Point2f> points1, points2;
        for (size_t i = 0; i < goodmatches.size(); i++)
        {
            {
                points1.push_back(keypoints1[goodmatches[i].queryIdx].pt);
                points2.push_back(keypoints2[goodmatches[i].trainIdx].pt);
            }
        }
        cv::Mat result;
        mergeImage(result,points1,points2,middle);
        setCurrentImage(result);
    }
    else{
        setCurrentImage(images[middle]);
    }
    
    
}
void Stitcher::addImage(const std::string &filename)
{
    cv::Mat image = cv::imread(filename);
    images.emplace_back(image);
}

void Stitcher::mergeImage(cv::Mat& result,std::vector<cv::Point2f>& points1 ,std::vector<cv::Point2f>& points2, uint8_t middle){
        cv::Mat H = cv::findHomography(points2, points1, cv::RANSAC, 8,cv::noArray(),10000,0.999);
        cv::warpPerspective(images[middle + 1], result, H, cv::Size(images[middle].cols + images[middle + 1].cols, std::max(images[middle + 1].rows, images[middle].rows)));
        cv::Mat half(result, cv::Rect(0, 0, images[middle].cols, images[middle].rows));
        images[middle].copyTo(half);
        currentStitchedImage = result;
        cv::imshow("result",result);
        cv::waitKey(0);
}

void Stitcher::setCurrentImage(cv::Mat& image){
    currentStitchedImage = image;
}