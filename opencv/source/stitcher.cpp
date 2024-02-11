#include "../header/stitcher.hpp"

Stitcher::Stitcher(std::vector<cv::Mat> &images, std::vector<cv::Mat> &grayscaledImages, cv::Mat &currentStitchedImage, std::vector<cv::Mat> &imagesDescriptors, std::vector<std::vector<cv::KeyPoint>> &imagesKeypoints, cv::Mat &currentImageDescriptor, cv::Ptr<cv::Feature2D> &orb, std::vector<cv::KeyPoint> &currentImageKeypoints)
    : images(images), grayscaledImages(grayscaledImages),
      currentStitchedImage(currentStitchedImage),
      imagesKeypoints(imagesKeypoints),
      imagesDescriptors(imagesDescriptors), orb(orb), currentImageDescriptor(currentImageDescriptor), currentImageKeypoints(currentImageKeypoints) {}

bool checkInteriorExterior(const cv::Mat &mask, const cv::Rect &croppingMask, int &top, int &bottom, int &left, int &right)
{
    // Return true if the rectangle is fine as it is
    bool result = true;

    cv::Mat sub = mask(croppingMask);
    int x = 0;
    int y = 0;
    // Count how many exterior pixels are, and choose that side for reduction where most exterior pixels occurred (that's the heuristic)
    int top_row = 0;
    int bottom_row = 0;
    int left_column = 0;
    int right_column = 0;
    for (y = 0, x = 0; x < sub.cols; ++x)
    {
        // If there is an exterior part in the interior we have to move the top side of the rect a bit to the bottom
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++top_row;
        }
    }
    for (y = (sub.rows - 1), x = 0; x < sub.cols; ++x)
    {
        // If there is an exterior part in the interior we have to move the bottom side of the rect a bit to the top
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++bottom_row;
        }
    }
    for (y = 0, x = 0; y < sub.rows; ++y)
    {
        // If there is an exterior part in the interior
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++left_column;
        }
    }
    for (x = (sub.cols - 1), y = 0; y < sub.rows; ++y)
    {
        // If there is an exterior part in the interior
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++right_column;
        }
    }
    // The idea is to set `top = 1` if it's better to reduce the rect at the top than anywhere else.
    if (top_row > bottom_row)
    {
        if (top_row > left_column)
        {
            if (top_row > right_column)
            {
                top = 1;
            }
        }
    }
    else if (bottom_row > left_column)
    {
        if (bottom_row > right_column)
        {
            bottom = 1;
        }
    }
    if (left_column >= right_column)
    {
        if (left_column >= bottom_row)
        {
            if (left_column >= top_row)
            {
                left = 1;
            }
        }
    }
    else if (right_column >= top_row)
    {
        if (right_column >= bottom_row)
        {
            right = 1;
        }
    }
    return result;
}

bool compareX(cv::Point a, cv::Point b)
{
    return a.x < b.x;
}

bool compareY(cv::Point a, cv::Point b)
{
    return a.y < b.y;
}

cv::Mat crop(cv::Mat &image)
{
    cv::Mat gray;
    image.convertTo(image, CV_8U);
    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
    // Extract all the black background (and some interior parts maybe)
    cv::Mat mask = gray > 0;
    // now extract the outer contour
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));
    cv::Mat contourImage = cv::Mat::zeros(image.size(), CV_8UC3);
    ;
    // Find contour with max elements
    int maxSize = 0;
    int id = 0;
    for (int i = 0; i < contours.size(); ++i)
    {
        if (contours.at((unsigned long)i).size() > maxSize)
        {
            maxSize = (int)contours.at((unsigned long)i).size();
            id = i;
        }
    }
    // Draw filled contour to obtain a mask with interior parts
    cv::Mat contourMask = cv::Mat::zeros(image.size(), CV_8UC1);
    cv::drawContours(contourMask, contours, id, 255, -1, 8, hierarchy, 0, cv::Point());
    // Sort contour in x/y directions to easily find min/max and next
    std::vector<cv::Point> cSortedX = contours.at((unsigned long)id);
    std::sort(cSortedX.begin(), cSortedX.end(), compareX);
    std::vector<cv::Point> cSortedY = contours.at((unsigned long)id);
    std::sort(cSortedY.begin(), cSortedY.end(), compareY);
    int minXId = 0;
    int maxXId = (int)(cSortedX.size() - 1);
    int minYId = 0;
    int maxYId = (int)(cSortedY.size() - 1);
    cv::Rect croppingMask;
    while ((minXId < maxXId) && (minYId < maxYId))
    {
        cv::Point min(cSortedX[minXId].x, cSortedY[minYId].y);
        cv::Point max(cSortedX[maxXId].x, cSortedY[maxYId].y);
        croppingMask = cv::Rect(min.x, min.y, max.x - min.x, max.y - min.y);
        // Out-codes: if one of them is set, the rectangle size has to be reduced at that border
        int ocTop = 0;
        int ocBottom = 0;
        int ocLeft = 0;
        int ocRight = 0;
        bool finished = checkInteriorExterior(contourMask, croppingMask, ocTop, ocBottom, ocLeft, ocRight);
        if (finished == true)
            break;
        // Reduce rectangle at border if necessary
        if (ocLeft)
        {
            ++minXId;
        }
        if (ocRight)
        {
            --maxXId;
        }
        if (ocTop)
        {
            ++minYId;
        }
        if (ocBottom)
        {
            --maxYId;
        }
    }
    // Crop image with created mask
    return image(croppingMask);
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
    cv::Mat croppedimg = crop(currentStitchedImage);
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