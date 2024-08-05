#include "../header/stitcher.hpp"
#include "../header/imageproc.hpp"

int main(int argc, char *argv[]) {
    Stitcher stitch;
    for(int i = 1; i < argc; i++){
        const cv::Mat img = cv::imread(argv[i], cv::IMREAD_COLOR);
        cv::Mat resized;
        cv::Size size(800, img.rows * 600 / img.cols);
        cv::resize(img, resized, size);
        if(i==1) stitch = Stitcher(resized);
        else stitch.addImage(resized);
    }
    stitch.StitchImages();
    return 0;
}