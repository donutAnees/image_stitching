#include "../header/stitcher.hpp"

Stitcher::Stitcher(std::vector<cv::Mat> &images, std::vector<cv::Mat> &grayscaledImages, cv::Mat &currentStitchedImage, std::vector<cv::KeyPoint> &currentImageKeypoints, cv::Mat &currentImageDescriptor, cv::Ptr<cv::Feature2D> &orb)
    : images(images), grayscaledImages(grayscaledImages),
      currentStitchedImage(currentStitchedImage),
      currentImageKeypoints(currentImageKeypoints),
      currentImageDescriptor(currentImageDescriptor), orb(orb) {}

void Stitcher::processImage()
{
    uint8_t imageCount = images.size();
    uint8_t middle = imageCount / 2 - 1;
    uint8_t previousIndexToMerge = middle - 1;
    uint8_t nextIndexToMerge = middle + 1;
    if (imageCount % 2 == 0)
    {
        cv::Mat descriptors1, descriptors2;
        std::vector<cv::KeyPoint> keypoints1, keypoints2;
        orb->detectAndCompute(images[middle], cv::noArray(), keypoints1, descriptors1);
        orb->detectAndCompute(images[middle + 1], cv::noArray(), keypoints2, descriptors2);
        std::vector<cv::Point2f> points1, points2;
        getMatchingPoint(points1, points2, descriptors1, descriptors2, keypoints1, keypoints2);
        cv::Mat result;
        mergeMiddleImages(result, points1, points2, middle);
        setCurrentImage(result);
        nextIndexToMerge++;
    }
    else
    {
        setCurrentImage(images[middle]);
    }

    // while(previousIndexToMerge > 0){
    //     mergeLeftMidRightImages(images[previousIndexToMerge],images[nextIndexToMerge]);
    //     previousIndexToMerge--;
    //     nextIndexToMerge++;
    // }
}
void Stitcher::addImage(const std::string &filename)
{
    cv::Mat image = cv::imread(filename);
    images.emplace_back(image);
}

void Stitcher::mergeMiddleImages(cv::Mat &result, std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, uint8_t middle)
{
    cv::Mat H = cv::findHomography(points2, points1, cv::RANSAC, 8, cv::noArray(), 10000, 0.999);
    cv::warpPerspective(images[middle + 1], result, H, cv::Size(images[middle].cols + images[middle + 1].cols, std::max(images[middle + 1].rows, images[middle].rows)));
    cv::Mat half(result, cv::Rect(0, 0, images[middle].cols, images[middle].rows));
    images[middle].copyTo(half);
    currentStitchedImage = result;
    cv::imwrite("result.jpg", result);
}

void Stitcher::mergeMidRightImages(std::vector<cv::Point2f>& points1 ,std::vector<cv::Point2f>& points2, cv::Mat &rightImage)
{
    cv::Mat result;
    cv::Mat H = cv::findHomography(points2, points1, cv::RANSAC, 8, cv::noArray(), 10000, 0.999);
    cv::warpPerspective(rightImage, result, H, cv::Size(currentStitchedImage.cols + rightImage.cols, std::max(currentStitchedImage.rows, rightImage.rows)));
    cv::Mat half(result, cv::Rect(0, 0, currentStitchedImage.cols, currentStitchedImage.rows));
    currentStitchedImage.copyTo(half);
    setCurrentImage(result);
}

void Stitcher::mergeLeftMidImages(std::vector<cv::Point2f>& points1 ,std::vector<cv::Point2f>& points2, cv::Mat &leftImage)
{
    cv::Mat result;
    cv::Mat H = cv::findHomography(points1, points2, cv::RANSAC, 8, cv::noArray(), 10000, 0.999);
    cv::warpPerspective(leftImage, result, H, cv::Size(leftImage.cols + currentStitchedImage.cols, std::max(leftImage.rows, currentStitchedImage.rows)));
    cv::Mat half(result, cv::Rect(leftImage.cols, 0, leftImage.cols + currentStitchedImage.cols, currentStitchedImage.rows));
    currentStitchedImage.copyTo(half);
    setCurrentImage(result);
}

void Stitcher::setCurrentImage(cv::Mat &image)
{
    currentStitchedImage = image;
    orb->detectAndCompute(currentStitchedImage, cv::noArray(), currentImageKeypoints, currentImageDescriptor);
}

void Stitcher::showMatches(std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2, std::vector<cv::DMatch> &goodmatches, int middle)
{
    cv::Mat imMatches;
    cv::drawMatches(images[middle], keypoints1, images[middle + 1], keypoints2, goodmatches, imMatches);
    cv::imshow("result", imMatches);
    cv::waitKey(0);
}

void Stitcher::mergeLeftMidRightImages(cv::Mat &leftImage, cv::Mat &rightImage)
{
    cv::Mat lDescriptors, rDescriptors2;
    std::vector<cv::KeyPoint> lKeypoints1, rKeypoints2;
    orb->detectAndCompute(leftImage, cv::noArray(), lKeypoints1, lDescriptors);
    orb->detectAndCompute(rightImage, cv::noArray(), rKeypoints2, rDescriptors2);
}

void Stitcher::getMatchingPoint(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, cv::Mat &descriptors1, cv::Mat &descriptors2, std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2)
{
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

    for (size_t i = 0; i < goodmatches.size(); i++)
    {
        {
            points1.push_back(keypoints1[goodmatches[i].queryIdx].pt);
            points2.push_back(keypoints2[goodmatches[i].trainIdx].pt);
        }
    }
}