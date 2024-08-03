#include "../header/stitcher.hpp"
#include "../header/imageproc.hpp"

int main(int argc, char *argv[]) {
    auto start = std::chrono::steady_clock::now();
    int count = 0; // the number of images
    std::cin >> count;
    Stitcher stitch;
    for(int i = 0; i < count; i++){
        std::string filename;
        std::cin >> filename;
        const cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
        cv::Mat resized;
        cv::Size size(800, img.rows * 600 / img.cols);
        cv::resize(img, resized, size);
        if(i==0) stitch = Stitcher(resized);
        else stitch.addImage(resized);
    }
    stitch.StitchImages();
    auto end = std::chrono::steady_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl; 
    return 0;
}