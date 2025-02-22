#ifndef AFFINE_TRANSFORMATION_CLASSIFY_HPP
#define AFFINE_TRANSFORMATION_CLASSIFY_HPP

#include "armor.hpp"
#include "detector.hpp"
#include "transform.hpp"
// OpenCV
#include <opencv2/opencv.hpp>

// STD
#include <cmath>
#include <string>
#include <vector>

namespace rm_auto_aim
{
    class Classify
    {
    public:
       static bool ReadAndSaveImages(const cv::String& inputPattern);
    };

}
#endif //AFFINE_TRANSFORMATION_CLASSIFY_HPP
