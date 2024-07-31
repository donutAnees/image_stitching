#include "../header/stitcher.hpp"

const cv::Ptr<cv::Feature2D>  Stitcher::descriptor = cv::ORB::create();
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
    cv::imwrite("result.jpg", imMatches);
}

void Stitcher::print()
{
    cv::imwrite("result.jpg", pano.first);
}

cv::Point shift(cv::Mat &img, cv::Rect rect) {
  int top = 0, bottom = 0, left = 0, right = 0;
  cv::Point tl = rect.tl();
  cv::Point br = rect.br();
  if (tl.y < 0)
    top = -tl.y;
  if (br.y > img.rows)
    bottom = br.y - img.rows;
  if (tl.x < 0)
    left = -tl.x;
  if (br.x > img.cols)
    right = br.x - img.cols;
  cv::copyMakeBorder(img, img, top, bottom, left, right, cv::BORDER_CONSTANT);
  return cv::Point(left, top);
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
    pano = std::make_pair(img, cv::Point(0, 0));
}

void Stitcher::extract(const cv::Mat &img)
{
    images.push_back(img.clone());
    // convert to grayscale
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    grayscaledImages.push_back(gray);
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
    // showMatches(imagesKeypoints[n-2], imagesKeypoints[n-1], matches, images[n-1], images.back());
    return std::make_pair(trainIDs, queryIDs);
}

void Stitcher::addImage(const cv::Mat& img) {
    extract(img);
    int n = images.size();
    std::pair<std::vector<int>, std::vector<int>> currentmatches = getMatchingPoint(imagesDescriptor[n-2],imagesDescriptor[n-1]);
    std::pair<cv::Mat, std::vector<bool>> homography = getHomography(currentmatches.first, currentmatches.second, getPoints(imagesKeypoints[n-2]), getPoints(imagesKeypoints[n-1]));
    matches.push_back(currentmatches);
    homographies.push_back(homography.first);
    true_matches.push_back(homography.second);
    const cv::Mat &prevPano = pano.first;
    const cv::Point &prevOrig = pano.second;
    const std::vector<int> &trainIDs = matches.back().first;
    const std::vector<int> &queryIDs = matches.back().second;
    std::vector<cv::Point2f> trainPts = getPoints(imagesKeypoints[imagesKeypoints.size() - 2]);
    std::vector<cv::Point2f> queryPts = getPoints(imagesKeypoints.back());
    for (cv::Point2f &pt : trainPts)
        pt += (cv::Point2f) pano.second;
    cv::Mat corrH = getHomography(trainIDs, queryIDs, trainPts, queryPts).first;
    pano = getPano(img,prevPano,corrH);
}

std::pair<cv::Mat, std::vector<bool>> Stitcher::getHomography(const std::vector<int> &trainIDs, const std::vector<int> &queryIDs, const std::vector<cv::Point2f> &trainPts, const std::vector<cv::Point2f> &queryPts)
{
    std::vector<cv::Point2f> srcPoints, dstPoints;
    for (size_t i = 0; i < trainIDs.size(); i++) {
        srcPoints.push_back(trainPts[trainIDs[i]]);
        dstPoints.push_back(queryPts[queryIDs[i]]);
    }
    cv::Mat mask;
    cv::Mat H = cv::estimateAffine2D(srcPoints, dstPoints, mask);
    int rows = mask.rows;
    std::vector<bool> state(rows);
    for (int i = 0; i < rows; i++)
        state[i] = mask.at<bool>(i, 0);
    H.push_back(cv::Mat((cv::Mat1d(1, 3) << 0, 0, 1)));
    return std::make_pair(H, state);
}

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
  cv::Rect roi = warpRect(img.size(), H);
  // Transform the origin
  cv::Mat T = (cv::Mat_<double>(3, 3) << 1, 0, -orig.x, 0, 1, -orig.y, 0, 0, 1);
  cv::Mat corrH = T * H;
  cv::Mat warped;
  cv::Size dsize = warpRect(img.size(), corrH).size(); 
  warpPerspective(img, warped, corrH, dsize);
  return warped;
}

std::pair<cv::Mat, cv::Point> Stitcher::getPano(const cv::Mat &img, const cv::Mat &lastPano, const cv::Mat &H){
    cv::Point orig = warpRect(lastPano.size(), H).tl(); 
    cv::Mat currentpano = warpImage(img,H,orig);
    cv::Rect newRect(-orig, img.size());
    cv::Point shiftedPoint = shift(currentpano,newRect);
    img.copyTo(currentpano(cv::Rect(shiftedPoint-orig,img.size())));
    cv::Rect blackborder = crop(currentpano);
    currentpano = currentpano(blackborder);
    cv::Point newOrig = shiftedPoint - orig - blackborder.tl();
    return std::make_pair(currentpano, newOrig);
}