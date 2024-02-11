#include "../header/stitcher.hpp"

Stitcher::Stitcher(std::vector<cv::Mat> &images, std::vector<cv::Mat> &grayscaledImages, cv::Mat &currentStitchedImage, std::vector<cv::Mat> &imagesDescriptors, std::vector<std::vector<cv::KeyPoint>> &imagesKeypoints, cv::Mat &currentImageDescriptor, cv::Ptr<cv::Feature2D> &orb, std::vector<cv::KeyPoint> &currentImageKeypoints)
    : images(images), grayscaledImages(grayscaledImages),
      currentStitchedImage(currentStitchedImage),
      imagesKeypoints(imagesKeypoints),
      imagesDescriptors(imagesDescriptors), orb(orb), currentImageDescriptor(currentImageDescriptor), currentImageKeypoints(currentImageKeypoints) {}

cv::Rect findLargestContourRect(const cv::Mat &img)
{
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    auto largest_contour = std::max_element(contours.begin(), contours.end(), [](const std::vector<cv::Point> &a, const std::vector<cv::Point> &b)
                                            { return cv::contourArea(a) < cv::contourArea(b); });
    cv::Rect maxRect = cv::boundingRect(*largest_contour);
    return maxRect;
}

cv::Rect findNonBlackRegion(const cv::Mat &img)
{
    cv::Mat gray, binary;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY);
    cv::Mat mask = cv::Mat::zeros(binary.size(), CV_8UC1);
    cv::Rect roi = findLargestContourRect(binary);
    cv::rectangle(mask, roi, 255);
    // create 2 copies of mask to have our actual minRectangle region and to count how many pixels to subtract
    cv::Mat minRect = mask.clone();
    cv::Mat sub = mask.clone();
    cv::Mat kernel = cv::Mat::ones(1, 1, CV_8U);
    while (cv::countNonZero(sub) > 0)
    {
        cv::erode(minRect, minRect, kernel);
        cv::subtract(minRect, binary, sub);
    }
    roi = findLargestContourRect(minRect);
    return roi;
}

cv::Mat cropToNonBlackRegion(const cv::Mat &img)
{
    cv::Mat croppedimg;
    cv::Rect boundary = findNonBlackRegion(img);
    croppedimg = img(boundary);
    return croppedimg;
}

void Stitcher::processImage()
{
    uint8_t imageCount = images.size();
    uint8_t middle = imageCount / 2 - 1;
    int previousIndexToMerge;
    int nextIndexToMerge;
    if (imageCount % 2 == 0)
    {
        std::vector<cv::Point2f> points1, points2;
        getMatchingPoint(points1, points2, imagesDescriptors[middle], imagesDescriptors[middle + 1], imagesKeypoints[middle], imagesKeypoints[middle + 1], grayscaledImages[middle], grayscaledImages[middle + 1]);
        cv::Mat result;
        mergeMiddleImages(result, points1, points2, middle);
        setCurrentImage(result);
        previousIndexToMerge = middle - 1;
        nextIndexToMerge = middle + 2;
    }
    else
    {
        middle = middle + 1;
        previousIndexToMerge = middle - 1;
        nextIndexToMerge = middle + 1;
        setCurrentImage(images[middle]);
    }
    while (previousIndexToMerge >= 0)
    {
        mergeLeftMidRightImages(previousIndexToMerge, nextIndexToMerge);
        previousIndexToMerge--;
        nextIndexToMerge++;
    }
    cv::Mat croppedimg = cropToNonBlackRegion(currentStitchedImage);
    cv::imwrite("result.jpg", croppedimg);
}

void Stitcher::addImage(const std::string &filename, bool flag)
{
    cv::Mat image = cv::imread(filename);
    if (flag)
    {
        cv::Size newSize(image.cols / 4, image.rows / 4);
        cv::resize(image, image, newSize);
    }
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat descriptors;
    std::vector<cv::KeyPoint> keypoints;
    orb->detectAndCompute(gray, cv::noArray(), keypoints, descriptors);
    images.emplace_back(image);
    imagesKeypoints.emplace_back(keypoints);
    imagesDescriptors.emplace_back(descriptors);
    grayscaledImages.emplace_back(gray);
}

void Stitcher::setCurrentImage(cv::Mat &image)
{
    currentStitchedImage = image;
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    orb->detectAndCompute(gray, cv::noArray(), currentImageKeypoints, currentImageDescriptor);
}

void Stitcher::showMatches(std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2, std::vector<cv::DMatch> &goodmatches, cv::Mat &image1, cv::Mat &image2)
{
    cv::Mat imMatches;
    cv::drawMatches(image1, keypoints1, image2, keypoints2, goodmatches, imMatches);
    cv::imshow("result", imMatches);
    cv::waitKey(0);
}

void Stitcher::mergeMiddleImages(cv::Mat &result, std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, uint8_t middle)
{
    cv::Mat H = cv::findHomography(points2, points1, cv::RANSAC, 8, cv::noArray(), 10000, 0.999);
    cv::warpPerspective(images[middle + 1], result, H, cv::Size(images[middle].cols + images[middle + 1].cols, std::max(images[middle + 1].rows, images[middle].rows)));
    cv::Mat half(result, cv::Rect(0, 0, images[middle].cols, images[middle].rows));
    images[middle].copyTo(half);
    currentStitchedImage = result;
}

void Stitcher::mergeMidRightImages(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, int rightIndex)
{
    cv::Mat result;
    cv::Mat H = cv::findHomography(points2, points1, cv::RANSAC, 8, cv::noArray(), 10000, 0.999);
    cv::warpPerspective(images[rightIndex], result, H, cv::Size(currentStitchedImage.cols + images[rightIndex].cols, std::max(currentStitchedImage.rows, images[rightIndex].rows)));
    cv::Mat half(result, cv::Rect(0, 0, currentStitchedImage.cols, currentStitchedImage.rows));
    currentStitchedImage.copyTo(half);
    setCurrentImage(result);
}

void Stitcher::mergeLeftMidImages(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, int leftIndex)
{
    cv::Mat result;
    cv::Mat H = cv::findHomography(points1, points2, cv::RANSAC, 8, cv::noArray(), 10000, 0.999);
    cv::Rect roi = findWrapRect(currentStitchedImage.size(), H);
    cv::Mat T = (cv::Mat_<double>(3, 3) << 1, 0, -roi.x, 0, 1, -roi.y, 0, 0, 1);
    cv::warpPerspective(images[leftIndex], result, T * H, cv::Size(roi.width + currentStitchedImage.cols, std::max(roi.height, currentStitchedImage.cols)));
    cv::Mat half(result, cv::Rect(-roi.x, -roi.y, currentStitchedImage.cols, currentStitchedImage.rows));
    currentStitchedImage.copyTo(half);
    setCurrentImage(result);
}

void Stitcher::mergeLeftMidRightImages(int leftIndex, int rightIndex)
{
    cv::Mat lDescriptors = imagesDescriptors[leftIndex], rDescriptors = imagesDescriptors[rightIndex];
    std::vector<cv::KeyPoint> lKeypoints = imagesKeypoints[leftIndex], rKeypoints = imagesKeypoints[rightIndex];
    std::vector<cv::Point2f> lpoints, rpoints, lmpoints, rmpoints;
    getMatchingPoint(rpoints, rmpoints, currentImageDescriptor, rDescriptors, currentImageKeypoints, rKeypoints, currentStitchedImage, images[rightIndex]);
    mergeMidRightImages(rpoints, rmpoints, rightIndex);
    getMatchingPoint(lpoints, lmpoints, lDescriptors, currentImageDescriptor, lKeypoints, currentImageKeypoints, images[leftIndex], currentStitchedImage);
    mergeLeftMidImages(lpoints, lmpoints, leftIndex);
}

void Stitcher::getMatchingPoint(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, cv::Mat &descriptors1, cv::Mat &descriptors2, std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2, cv::Mat &image1, cv::Mat &image2)
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
            points1.emplace_back(keypoints1[goodmatches[i].queryIdx].pt);
            points2.emplace_back(keypoints2[goodmatches[i].trainIdx].pt);
        }
    }
    // showMatches(keypoints1, keypoints2, goodmatches, image1, image2);
}

cv::Rect Stitcher::findWrapRect(cv::Size sz, cv::Mat &H)
{
    std::vector<cv::Point2f> corners(4);
    int w = sz.width;
    int h = sz.height;
    corners[0] = cv::Point2f(0, 0);
    corners[1] = cv::Point2f(w, 0);
    corners[2] = cv::Point2f(0, h);
    corners[3] = cv::Point2f(w, h);
    std::vector<cv::Point2f> warpedCorners;
    cv::perspectiveTransform(corners, warpedCorners, H);
    return cv::boundingRect(warpedCorners);
}