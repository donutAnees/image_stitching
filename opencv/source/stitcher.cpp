#include "../header/stitcher.hpp"
#include "../header/imageproc.hpp"

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
    pano = std::make_pair(img, cv::Point(0, 0));
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
    // Adjust the position of the keypoints from the previous image to match their actual position in the latest panorama.
    // This step is necessary because the latest panorama might have been shifted or transformed in the coordinate system
    // of the final panorama.
    for (cv::Point2f &pt : trainPts)
        pt += (cv::Point2f) prevOrig;
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
    double reprojThresh = 4.0;
    cv::Mat mask;
    cv::Mat H = cv::findHomography(srcPoints, dstPoints, cv::RANSAC, reprojThresh, mask);
    int rows = mask.rows;
    std::vector<bool> state(rows);
    for (int i = 0; i < rows; i++)
        state[i] = mask.at<bool>(i, 0);
    return std::make_pair(H, state);
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

std::pair<cv::Mat, cv::Point> Stitcher::getPano(const cv::Mat &img, const cv::Mat &lastPano, const cv::Mat &H){
    cv::Point orig = warpRect(lastPano.size(), H).tl(); 
    cv::Mat currentpano = warpImage(lastPano,H,orig);
    cv::Rect newRect(-orig, img.size());
    cv::Point shiftedPoint = shift(currentpano,newRect);
    img.copyTo(currentpano(cv::Rect(shiftedPoint-orig,img.size())));
    cv::Rect blackborder = crop(currentpano);
    currentpano = currentpano(blackborder);
    cv::Point newOrig = shiftedPoint - orig - blackborder.tl();
    return std::make_pair(currentpano, newOrig);
}

std::pair<cv::Mat, cv::Point> Stitcher::getPano(){
  return pano;
}