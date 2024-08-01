#include "../header/stitcher.hpp"
#include "../header/imageproc.hpp"

cv::Mat stitch(const std::pair<cv::Mat, cv::Point> &prevPano, const std::pair<cv::Mat, cv::Point> &nextPano) {
  const cv::Mat &prevImg = prevPano.first;
  const cv::Mat &nextImg = nextPano.first;
  const cv::Point &prevOrig = prevPano.second;
  const cv::Point &nextOrig = nextPano.second;
  cv::Point prevbr = prevImg.size();
  cv::Point nextbr = nextImg.size();
  cv::Point nextdiag = nextbr - nextOrig;
  std::vector<cv::Point> cornerpoints(4);
  cornerpoints[0] = cv::Point(0, 0);
  cornerpoints[1] = prevbr;
  cornerpoints[2] = prevOrig + nextdiag;
  cornerpoints[3] = prevOrig - nextOrig;
  cv::Rect bb = cv::boundingRect(cornerpoints);
  cv::Mat pano = prevImg.clone();
  cv::Point shiftedPoint = shift(pano, bb);
  cv::Point orig = prevOrig + shiftedPoint;
  nextImg.copyTo(pano(cv::Rect(orig - nextOrig, nextImg.size())),nextImg);
  return pano;
}

int main(int argc, char *argv[]) {
    auto start = std::chrono::steady_clock::now();
    int count = 0; // the number of images
    std::cin >> count;
    std::vector<std::string> files;
    std::vector<std::string> filenames;
    for(int i = 0; i < count; i++){
        std::string filename;
        std::cin >> filename;
        filenames.push_back(filename);
    }

    auto end = std::chrono::steady_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl; 
    return 0;
}