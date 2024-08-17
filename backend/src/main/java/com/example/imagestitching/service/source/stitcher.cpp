#include "../header/stitcher.hpp"

const cv::Ptr<cv::Feature2D>  Stitcher::descriptor = cv::ORB::create(10000);
const cv::BFMatcher Stitcher::matcher = cv::BFMatcher(cv::NORM_HAMMING2);

std::vector<cv::Point2f> getPoints(std::vector<cv::KeyPoint> &kps) {
    std::vector<cv::Point2f> res(kps.size());
    for (size_t i = 0; i < kps.size(); i++)
        res[i] = kps[i].pt;
    return res;
}

void showMatches(std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2, const std::vector<std::vector<cv::DMatch>> &goodmatches, cv::Mat &image1, cv::Mat &image2)
{
    cv::Mat imMatches;
    cv::drawMatches(image1, keypoints1, image2, keypoints2, goodmatches, imMatches);
    cv::imshow("result.jpg", imMatches);
    cv::waitKey(0);
}

cv::Rect crop(const cv::Mat &img) {
  std::vector<cv::Point> nonBlackList;
  nonBlackList.reserve(img.total());
  for (int j = 0; j < img.rows; j++)
    for (int i = 0; i < img.cols; i++)
      if (img.at<cv::Vec3b>(j, i) != cv::Vec3b(0, 0, 0))
        nonBlackList.push_back(cv::Point(i, j));
  return cv::boundingRect(nonBlackList);
}

Stitcher::Stitcher(){};

Stitcher::Stitcher(const cv::Mat &img){
    extract(img);
    homographies.push_back(cv::Mat::eye(3, 3, CV_64F)); // Identity Matrix
}

/* The checkInteriorExterior function determines whether a given sub-matrix of an image contains exterior (0) pixels by checking the edges.*/
bool checkInteriorExterior(const cv::Mat &mask, const cv::Rect &croppingMask, int &top, int &bottom, int &left, int &right)
{
    bool result = true;
    cv::Mat sub = mask(croppingMask);
    int x = 0;
    int y = 0;
    int top_row = 0;
    int bottom_row = 0;
    int left_column = 0;
    int right_column = 0;
    for (y = 0, x = 0; x < sub.cols; ++x)
    {
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++top_row;
        }
    }
    for (y = (sub.rows - 1), x = 0; x < sub.cols; ++x)
    {
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++bottom_row;
        }
    }
    for (y = 0, x = 0; y < sub.rows; ++y)
    {
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++left_column;
        }
    }
    for (x = (sub.cols - 1), y = 0; y < sub.rows; ++y)
    {
        if (sub.at<char>(y, x) == 0)
        {
            result = false;
            ++right_column;
        }
    }
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

// This function  finds the largest contour in an image, calculates its bounding box, and crops the image to that bounding box while ensuring minimal inclusion of 0 pixels.
cv::Mat cropBlack(cv::Mat &image, cv::Mat &gray)
{
    cv::Mat mask = gray > 0;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));
    cv::Mat contourImage = cv::Mat::zeros(image.size(), CV_8UC3);
    ;
    int maxSize = 0;
    int id = 0;
    for (int i = 0; i < contours.size(); ++i)
    {
        if (contours.at(i).size() > maxSize)
        {
            maxSize = (int)contours.at(i).size();
            id = i;
        }
    }
    cv::Mat contourMask = cv::Mat::zeros(image.size(), CV_8UC1);
    cv::drawContours(contourMask, contours, id, 255, -1, 8, hierarchy, 0, cv::Point());
    std::vector<cv::Point> cSortedX = contours.at(id);
    std::sort(cSortedX.begin(), cSortedX.end(), [](cv::Point a, cv::Point b)
              { return a.x < b.x; });
    std::vector<cv::Point> cSortedY = contours.at(id);
    std::sort(cSortedY.begin(), cSortedY.end(), [](cv::Point a, cv::Point b)
              { return a.y < b.y; });
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
        int ocTop = 0;
        int ocBottom = 0;
        int ocLeft = 0;
        int ocRight = 0;
        bool finished = checkInteriorExterior(contourMask, croppingMask, ocTop, ocBottom, ocLeft, ocRight);
        if (finished == true)
            break;
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
    return image(croppingMask);
}

void Stitcher::extract(const cv::Mat &img)
{
    images.push_back(img.clone());
    // convert to grayscale
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    // Detect and compute
    cv::Mat descriptors;
    std::vector<cv::KeyPoint> keypoints;
    Stitcher::descriptor->detectAndCompute(gray, cv::noArray(), keypoints, descriptors);
    imagesKeypoints.push_back(keypoints);
    imagesDescriptor.push_back(descriptors);
}

std::pair<std::vector<int>, std::vector<int>> Stitcher::getMatchingPoint(const cv::Mat &prev, const cv::Mat &current)
{
    std::vector<std::vector<cv::DMatch>> matches;
    matcher.knnMatch(current, prev, matches, 2);
    std::vector<int> trainIDs, queryIDs;
    double ratio = 0.75;
    for (std::vector<cv::DMatch> &m : matches) {
        if ((m.size() == 2) && (m[0].distance < m[1].distance * ratio)) {
        trainIDs.push_back(m[0].trainIdx);
        queryIDs.push_back(m[0].queryIdx);
        }
    }
    int n = images.size();
    //showMatches(imagesKeypoints[n-2], imagesKeypoints[n-1], matches, images[n-2], images.back());
    return std::make_pair(trainIDs, queryIDs);
}

void Stitcher::addImage(const cv::Mat& img) {
    extract(img);
    int n = images.size();
    std::pair<std::vector<int>, std::vector<int>> currentmatches = getMatchingPoint(imagesDescriptor[n-2],imagesDescriptor[n-1]);
    cv::Mat homography = getHomography(currentmatches.second, currentmatches.first, getPoints(imagesKeypoints[n-1]), getPoints(imagesKeypoints[n-2]));
    matches.push_back(currentmatches);
    // We store previous cumulative homography * current homography
    homographies.push_back(homographies.back() * homography);
}

/* This function calculates the bounding rectangle that encloses the
 * transformed version of an image after applying a homography transformation. */
cv::Rect Stitcher::warpRect(cv::Size sz, const cv::Mat &homography) {
  std::vector<cv::Point2f> corners(4);
  float width = static_cast<float>(sz.width);
  float height = static_cast<float>(sz.height);
  corners[0] = cv::Point2f(0, 0); // top left
  corners[1] = cv::Point2f(width, 0); // top right
  corners[2] = cv::Point2f(0, height); // bottom left
  corners[3] = cv::Point2f(width, height); // bottom right
  std::vector<cv::Point2f> warpedCorners;
  cv::perspectiveTransform(corners, warpedCorners, homography);
  return cv::boundingRect(warpedCorners);
}

cv::Mat Stitcher::warpImage(const cv::Mat &img, const cv::Mat &H,
                            cv::Point orig) {
  // Create a translation matrix to adjust the image's position in the panorama coordinate system.
  cv::Mat T = (cv::Mat_<double>(3, 3) << 1, 0, -orig.x, 0, 1, -orig.y, 0, 0, 1);
  cv::Mat corrH = T * H;
  cv::Mat warped;
  cv::Size dsize = warpRect(img.size(), corrH).size(); 
  warpPerspective(img, warped, corrH, dsize);
  return warped;
}

cv::Mat Stitcher::getHomography(const std::vector<int> &trainIDs, const std::vector<int> &queryIDs, const std::vector<cv::Point2f> &trainPts, const std::vector<cv::Point2f> &queryPts)
{
    std::vector<cv::Point2f> srcPoints, dstPoints;
    for (size_t i = 0; i < trainIDs.size(); i++) {
        srcPoints.push_back(trainPts[trainIDs[i]]);
        dstPoints.push_back(queryPts[queryIDs[i]]);
    }
    double reprojThresh = 4.0;
    cv::Mat mask;
    cv::Mat H = cv::findHomography(srcPoints, dstPoints, cv::RANSAC, reprojThresh, mask);
    return H;
}

void Stitcher::StitchImages() {
    cv::Mat prevPano = images[0].clone();
    cv::Mat mask;
    for (int i = 1; i < images.size(); ++i) {
        cv::Mat result;
        cv::warpPerspective(images[i], result, homographies[i], cv::Size(prevPano.cols + images[i].cols, std::max(prevPano.rows, images[i].rows)));
        cv::Mat half(result,cv::Rect(0, 0, prevPano.cols,prevPano.rows));
        prevPano.copyTo(half);
        cv::Mat gray;
        cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
        prevPano = cropBlack(result,gray);
    }
    cv::imwrite("result.jpg",prevPano);
}

void stitchImagesFromPaths(const std::vector<std::string>& paths) {
    Stitcher stitch;
    for (size_t i = 0; i < paths.size(); i++) {
        const cv::Mat img = cv::imread(paths[i], cv::IMREAD_COLOR);
        cv::Mat resized;
        cv::Size size(800, img.rows * 600 / img.cols);
        cv::resize(img, resized, size);
        if (i == 0) {
            stitch = Stitcher(resized);
        } else {
            stitch.addImage(resized);
        }
    }
    stitch.StitchImages();
}