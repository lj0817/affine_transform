#include "include/classify.hpp"
#include <opencv2/opencv.hpp>
#include <string>

int main(){
    cv::String inputPattern = "E:/clionProjects/affine_transformation/pictures/*.bmp";
    bool allImagesSaved =rm_auto_aim::Classify::ReadAndSaveImages(inputPattern);
    if (!allImagesSaved) {
        std::cout << "Some images failed to save." << std::endl;
        return -1;
    }
    std::cout << "All images saved successfully." << std::endl;
    return 0;
}
