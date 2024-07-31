#include "../header/stitcher.hpp"

int main(int argc, char *argv[]) {
    auto start = std::chrono::steady_clock::now();
    int count = 0; // the number of images
    std::cin >> count;
    std::vector<std::string> files;
    Stitcher stitcher;
    for(int i = 0; i < count; i++){
        std::string filename;
        std::cin >> filename;
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        if(i == 0) stitcher = Stitcher(image);
        else stitcher.addImage(image); 
    }
    stitcher.print();
    auto end = std::chrono::steady_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl; 
    return 0;
}